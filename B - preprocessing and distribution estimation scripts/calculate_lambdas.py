import pandas as pd
import numpy as np
import os
import math

        
def preprocess_df(df, sheet_name):
 
    df = df.copy()
    
    if 'maintenance_time' not in df.columns:
        return df

    maintenance_times = pd.to_datetime(df['maintenance_time'], errors='coerce', format='%Y/%m/%d %H:%M:%S')
    alarm_columns = df.columns[2:]
    # print(alarm_columns)
    
    for alarm in alarm_columns:
        for i in range(len(df)):
            if pd.isna(df.loc[i, alarm]):
                prev_time = maintenance_times[i]
                next_time = None
                for j in range(i + 1, len(df)):
                    if not pd.isna(maintenance_times[j]):
                        next_time = maintenance_times[j]
                        break
                if prev_time is not pd.NaT and next_time is not None:
                    time_diff_hours = (next_time - prev_time).total_seconds() / 3600
                    df.loc[i, alarm] = time_diff_hours

    # df.to_csv('result_prova_lambda\\'+sheet_name+'.csv', index=False)
    return df

def estimate_lambdas_for_alarms(all_dfs):
    
    # Estimate lambda for each alarm type across all DataFrames.
    combined_data = pd.concat(all_dfs, ignore_index=True)
    lambdas = {}
    alarm_columns = combined_data.columns[2:]  # Don't take the first 2 columns(fine guasto e maintinance_time)
    
    for alarm in alarm_columns:
        times_hours = combined_data[alarm]
        observed = times_hours.dropna()

        n_observed = len(observed)
        total_time_hours = observed.sum()

        if total_time_hours > 0:
            lambda_pre = n_observed / total_time_hours
            lambdas[alarm] = lambda_pre
        else:
            lambdas[alarm] = np.nan

    return lambdas
   
# Path to the folder containing the occurrences
folder_path = 'risultati_prima_ocorrenzza_PDM'

all_dfs = []
preprocess = int(input('Do you want to preprocess the diagnosted files? 1-yes, 2-no \n'))

# Process each Excel file in the folder
for file_name in os.listdir(folder_path):
    if file_name.endswith('.xlsx'):
        file_path = os.path.join(folder_path, file_name)
        xls = pd.ExcelFile(file_path)

        for sheet_name in xls.sheet_names:
            df = pd.read_excel(file_path, sheet_name=sheet_name)
            if preprocess == 1:
                df = preprocess_df(df, sheet_name)
            all_dfs.append(df)

# Estimate lambdas combining all data
lambdas = estimate_lambdas_for_alarms(all_dfs)

results_df = pd.DataFrame.from_dict(lambdas, orient='index', columns=['lambda'])

nome_path = 'risultati'

if folder_path == 'risultati_prima_ocorrenzza_PDM':
    nome_path = 'prima_ocorrenzza_PDM'

if preprocess == 1:  
    results_df.to_csv('calculate_statisctiche_' + nome_path + '_preprocess_threshold.csv')
else:
    results_df.to_csv('calculate_statisctiche_' + nome_path + '_threshold.csv')

print('Finish')
