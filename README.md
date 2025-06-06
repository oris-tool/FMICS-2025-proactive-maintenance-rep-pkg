# Data-Driven Synthesis of Stochastic Fault Trees for Proactive Maintenance of Railway Vehicles - Package

This repository is a companion page for the following publication:

**Laura Carnevali, Alessandro Fantechi, Gloria Gori, Denis Vreshtazi, Alessandro Borselli, Maria Rosaria Cefaloni and Lucio Rota.** 2025. Data-Driven Synthesis of Stochastic Fault Trees for Proactive Maintenance of Railway Vehicles. *Proceedings of the 30th International Conference on Formal Methods for Industrial Critical Systems (FMICS 2025)*

This package includes sample data, scripts and source code needed to understand and partially replicate our experimental results.

## Prerequisites

- Java 23 or higher
- Maven 3.8.0 or higher

## Setup Instructions

1. Clone this repository:
   ```bash
   git clone git@github.com:oris-tool/FMICS-2025-proactive-maintenance-rep-pkg
   cd FMICS-2025-proactive-maintenance-rep-pkg/C\ -\  faultflow
   ```

2. Run the installation script: 
   ```bash
   ./install.sh
   ```

3. Follow instruction in `./D\ -\ train\ reliability\ evaluation/`

## Content

This workflow implements a four-stage data processing pipeline. Sample raw data are provided in folder A.

1. **Stage 1 – Maintenance & Diagnostics Integration**: Merges corrective maintenance data, train composition, and diagnostic logs to find the first occurrence time (in hours) of each alarm after maintenance.

2. **Stage 2 – Alarm Statistics Estimation**: Processes these results to estimate the average alarm rate (λ) for each alarm type (folder B)

3. **Stage 3 – Fault Modeling in FaultFlow**: Use the estimated λ values to parameterize fault activation distributions in FaultFlow for model-driven dependability evaluation (folder C)

4. **Stage 4 – librbd**: Train reliability evaluation using the redundancy constraints provided for the given train configuration and the output of FaultFlow. (folder D)

## Input Files (Stage 1)

### 1. Maintenance File
- **File**: `maintenance.xlsx`
- **Structure**:
  - One sheet per train (e.g., TSR 008)
  - **Relevant columns**:
    - `Fine guasto`: End date of the maintenance event
    - `Sede Tecnica`: Identifies the affected coach
    - `Descr. assem. Padre`: Only rows with value `Trazione` are processed

### 2. Diagnostic Event Logs
- **Folder**: `Dati diagnostici`
- **Format**: One CSV per train (e.g., `TSR 008.csv`)
- **Relevant columns**:
  - `ts`: Unix timestamp (milliseconds), converted to datetime
  - `depot`, `event_type`, `machine_type`, `alert_type`, `cod`: used for filtering (see below)
  - `source` and `name`: used to associate events with specific coaches
  - `id`: diagnostic alarm ID

### 3. Train Composition File
- **File**: `composizione_treni.xlsx`
- **Structure**:
  - One sheet per train
  - First row: real coach names
  - Header: coach positions (e.g., first, second, ..., last)
  - The first coach name is used for filtering source in diagnostic logs

## Processing Workflow

### 1. Maintenance records preprocessing
- Only rows where `Descr. assem. Padre == 'Trazione'` are considered
- A `maintenance_time` column is computed in order to take in consideration the next day at 00:00 after the `Fine guasto` date:
  - Set to 00:00 of the day after the `Fine guasto` date

### 2. Diagnostic Data Preprocessing
- The diagnostic CSV is loaded and `ts` is converted to datetime
- **Filters applied** (we took in consideration only the alarms of the coaches that):
  - `depot == 0`
  - `event_type == "ON"`
  - `machine_type == "MD"`
  - `alert_type == "PDM"`
  - `cod == 5`

### 3. Integration with Train Composition
- Each train's composition sheet is read to get the list of coach names
- **Filtering logic**:
  - For the first coach, events must match both `source` and `name`
  - For other coaches, only `name` must match
- maintenance data is further filtered to match rows where `Sede Tecnica` equals the coach identifier

### 4. Alarm Timing Computation
- For each maintenance event:
  - Define an interval: from current `maintenance_time` to the next one (if any)
  - For each allowed alarm ID `{1, 2, 3, 4, 5, 6, 7, 8, 9, 11}`:
    - Find the first event in the diagnostic log that occurs in this interval
    - Compute the time difference (in hours) between the event and `maintenance_time`

## Output

- For each train, an Excel file is generated in the results folder:
  - **Filename**: e.g., `TSR 008_prima_occorrenza.xlsx`
  - One sheet per coach
  - Each sheet contains:
    - `Fine guasto` and `maintenance_time` for that coach
    - One column per allowed alarm, showing the delay (in hours) to first occurrence after maintenance

## Stage 2 – Alarm Rate Estimation (λ)

This second stage reads the Excel files produced in Stage 1 and calculates, for each alarm type, the estimated rate of occurrence λ using the formula:

```
λ = N/T
```

Where:
- **N** = number of observed alarms
- **T** = total observed time (sum of time intervals in hours)

### Optional Preprocessing
If an alarm is missing in a row (NaN), and another `maintenance_time` follows, it fills the value with the time difference between current and next maintenance. This assumes the alarm didn't occur in that interval but is censored.

### Workflow
1. Reads all Excel files in the folder `risultati_prima_ocorrenzza_PDM`
2. For each coach sheet, applies optional preprocessing (ask via input prompt)
3. Combines all coach data
4. Estimates λ for each alarm type
5. Exports a CSV:
   - **With or without preprocessing**:
     - `calculate_statisctiche_prima_ocorrenzza_PDM_preprocess_threshold.csv`
     - `calculate_statisctiche_prima_ocorrenzra_PDM_threshold.csv`

### Expected Folder/File:
- **maintenance file**: `maintenance.xlsx`
- **Diagnostic CSVs**: in folder `Dati diagnostici (giugno 2024 - con colonna pantografo)`
- **Composition file**: `composizione_treni.xlsx`
- The results folder is created automatically if it doesn't exist

## Stage 3 – Fault Modeling with FaultFlow (TCU System)

After estimating alarm occurrence rates (λ values) in Stage 2, they are manually integrated into a FaultFlow model of the Train Control Unit (TCU).

### Files Used

#### TCUSystemBuilder.java
This class defines the system model, including:
- Components and their structure
- Faults and their failure logic
- Injection of λ values to parameterize fault occurrence distributions

Each λ value corresponds to a specific alarm type (e.g., alarm ID 7) and is assigned to a fault as follows:

```java
double lambda7 = 0.0025; // example value from Stage 2 CSV
Fault alarm7 = new Fault("Alarm7");
alarm7.setOccurrence(new Exponential(1.0 / lambda7));
```

This configuration models alarm 7 as an exponentially-distributed fault with mean time to activation of 1/λ hours.

The process is repeated for all relevant alarms (e.g., 1, 2, 3, 4, 5, 6, 7, 8, 9, 11).

#### AnalysisLauncher.java
This file runs the evaluation using the FaultFlow engine. It performs the following:

1. **Instantiates the system** using TCUSystemBuilder:
   ```java
   Component system = new TCUSystemBuilder().build();
   ```

2. **Runs fault analysis**:
   ```java
   FaultFlow.runAnalysis(system, "output/failure_cdf.csv");
   ```

3. **Exports results**:
   - Failure duration distributions (.csv)
   - Importance measures (Birnbaum, Fussell-Vesely)
   - Execution time
   - Cut sets (if enabled)

### FaultFlow Output
After executing AnalysisLauncher, the following files are typically generated:
- `result_pyramis.csv`: cdf for key failure event
- `birnbaum.csv`, `fussell_vesely.csv`: importance measures for faults

## Requirements

- Python 3
- **Python libraries**:
  - pandas
  - openpyxl
- Java 11+ and Eclipse for FaultFlow (stage 3)

## How to Run

### 1. Run Stage 1: Maintenance-Diagnostics Integration
```bash
python script_prima_occorrenza_allarme.py
```

Make sure the following files/folders exist:
- `Avvisi SAP - Interventi manutentivi (Agg. giugno 2024).xlsx`
- `composizione_treni.xlsx`
- `Dati diagnostici (giugno 2024 - con colonna pantografo)/`

**Results go to**: `results/`

### 2. Run Stage 2: Lambda Estimation
```bash
python calculate_lambdas.py
```

At runtime, it will ask:
```
Do you want to preprocess the diagnosted files? 1-yes, 2-no
```

**Results can go to**:
- `calculate_statisctiche_prima_ocorrenzza_PDM_threshold.csv`
- `calculate_statisctiche_prima_ocorrenzza_PDM_preprocess_threshold.csv`

### 3. Run Stage 3:
Ensure you have:
- λ values correctly set in `TCUSystemBuilder.java` (copied from Stage 2 output)

1. Import the project in Eclipse
2. Run the analysis:
   `AnalysisLauncher.java` -> Run As -> Java Application

## Output Location

All results are saved in the `export/` directory. Outputs include:

| File | Description |
|------|-------------|
| `result-pyramis.csv` | CDF of the AZ system failure |
| `birnbaum.csv` | Birnbaum importance measures for internal faults |
| `fussell_vesely.csv` | Fussell-Vesely importance measures |

Using `plotter.py` it is possible to generate figures 3, 4 and 5.
