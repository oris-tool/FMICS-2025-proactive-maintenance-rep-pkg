import pandas as pd
import os
from datetime import datetime

# --- Settings ---
sap_file = 'maintenance.xlsx'
diag_folder = 'Dati diagnostici'
comp_file = 'composizione_treni.xlsx'  # File con la composizione dei treni
results_folder = 'results'
allowed_alarm_ids = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11}

# Crea la cartella results se non esiste
os.makedirs(results_folder, exist_ok=True)

# Carica il file SAP (con più sheet, uno per treno)
sap_excel = pd.ExcelFile(sap_file)

# Loop su ogni sheet (per ogni treno), saltando lo sheet "legenda colonne"
for train in sap_excel.sheet_names:
    if train.strip().lower() == 'legenda colonne':
        continue

    print(f"Processing train: {train}")

    # --- Carico la composizione del treno ---
    # Si assume che in ogni sheet del file di composizione:
    # - L'header (prima riga) contenga le posizioni (first, second, ..., last)
    # - La prima riga di dati (df_comp.iloc[0]) contenga i nomi reali delle carrozze
    df_comp = pd.read_excel(comp_file, sheet_name=train, header=0)
    # Otteniamo la lista dei nomi delle carrozze (escludendo eventuali NaN)
    composition_list = df_comp.iloc[0].dropna().tolist()
    #print(composition_list)                                               
    if not composition_list:
        print(f"Nessuna composizione trovata per {train}. Skip.")
        continue
    first_car = composition_list[0]
    
    # --- Carico e preparo il file SAP per il treno ---
    df_sap = sap_excel.parse(train)
    df_sap = df_sap[df_sap['Descr. assem. Padre'] == 'Trazione']                                                            
    df_sap['Fine guasto'] = pd.to_datetime(df_sap['Fine guasto'])
    df_sap = df_sap.sort_values(by='Fine guasto').reset_index(drop=True)
    # Calcola maintenance_time come il giorno dopo, alle 00:00
    df_sap['maintenance_time'] = (df_sap['Fine guasto'] + pd.Timedelta(days=1)).dt.normalize()
    
    # --- Carico il file diagnostico per il treno (CSV) ---
    diag_file = os.path.join(diag_folder, f"{train}.csv")
    if not os.path.exists(diag_file):
        print(f"File diagnostico {diag_file} non trovato. Skip {train}.")
        continue
    df_diag = pd.read_csv(diag_file, usecols=['source','name','machine_type','alert_type','ts','cod','id','id1','event_type','depot']) # low_memory=False)#, on_bad_lines='skip')
    # Converte la colonna 'ts' (Unix epoch in secondi) in datetime
    df_diag['date'] = pd.to_datetime(df_diag['ts'], unit='ms')
    
    # Filtraggio standard dei dati diagnostici
    df_diag_filtered = df_diag[
        (df_diag['depot'] == 0) &
        (df_diag['event_type'] == 'ON') &
        (df_diag['machine_type'] == 'MD') &
        (df_diag['alert_type'] == 'PDM') &
        (df_diag['cod'] == 5)
    ]

    # --- Prepara l'output per il treno ---
    # Creiamo un ExcelWriter per salvare il file di output del treno (uno sheet per carrozza)
    output_file = os.path.join(results_folder, f"{train}_prima_occorrenza.xlsx")
    with pd.ExcelWriter(output_file, engine='openpyxl') as writer:
        # Per ogni carrozza (coach) nella composizione
        for coach in composition_list:
            print(f"  Processing coach: {coach}")
            # Filtra le manutenzioni SAP: considera solo quelle in cui Sede Tecnica corrisponde al nome della carrozza
            df_sap_coach = df_sap[df_sap['Sede tecnica'] == coach].reset_index(drop=True)
            if df_sap_coach.empty:
                print(f"    Nessuna manutenzione trovata per la carrozza {coach}.")
                continue

            # Filtra i dati diagnostici in base al coach:
            # - Per il primo coach, richiede che il campo "source" sia uguale a coach e che "name" sia uguale a coach
            # - Per gli altri, si controlla solo che "name" sia uguale a coach
            if coach == first_car:
                df_diag_coach = df_diag_filtered[
                    (df_diag_filtered['source'] == coach) &
                    (df_diag_filtered['name'] == coach)
                ]
            else:
                df_diag_coach = df_diag_filtered[
                    (df_diag_filtered['source'] == first_car) &
                    (df_diag_filtered['name'] == coach)
                ]
            
            # Considera solo gli allarmi ammessi (in allowed_alarm_ids)
            unique_alarms = sorted(
                [a for a in df_diag_coach['id'].unique() if int(a) in allowed_alarm_ids],
                key=lambda x: int(x)
            )
            
            # Prepara un DataFrame di output per le manutenzioni di questa carrozza
            output_df = pd.DataFrame()
            output_df['Fine guasto'] = df_sap_coach['Fine guasto']
            output_df['maintenance_time'] = df_sap_coach['maintenance_time']
            print(df_sap_coach['maintenance_time'])
            
            results_list = []
            # Per ogni intervento (manutenzione) in df_sap_coach
            for i, row in df_sap_coach.iterrows():
                row_result = {}
                current_maintenance = row['maintenance_time']
                if pd.isna(current_maintenance):
                    for alarm in unique_alarms:
                        row_result[str(alarm)] = ""
                    results_list.append(row_result)
                    continue
                
                current_maintenance_ts = current_maintenance.timestamp()
                
                # Definisce l'intervallo: da current_maintenance a next_maintenance (se disponibile)
                if i < len(df_sap_coach) - 1:
                    next_maintenance = df_sap_coach.loc[i+1, 'maintenance_time']
                else:
                    next_maintenance = None
                
                for alarm in unique_alarms:
                    if next_maintenance is not None:
                        events = df_diag_coach[
                            (df_diag_coach['date'] >= current_maintenance) &
                            (df_diag_coach['date'] < next_maintenance) &
                            (df_diag_coach['id'] == alarm)
                        ]
                    else:
                        events = df_diag_coach[
                            (df_diag_coach['date'] >= current_maintenance) &
                            (df_diag_coach['id'] == alarm)
                        ]
                    if not events.empty:
                        events = events.sort_values(by='date')
                        first_event = events.iloc[0]['date']
                        # print(first_event)
                        # print('cm', current_maintenance)
                        diff_hours = (first_event - current_maintenance).total_seconds() / 3600.0
                        row_result[str(alarm)] = round(diff_hours, 2)
                    else:
                        row_result[str(alarm)] = ""
                results_list.append(row_result)
            
            alarms_df = pd.DataFrame(results_list)
            output_coach_df = pd.concat([output_df, alarms_df], axis=1)
            # Salva lo sheet con il nome della carrozza (coach)
            output_coach_df.to_excel(writer, sheet_name=str(coach), index=False)
    
    print(f"Output for train {train} saved in {output_file}")
