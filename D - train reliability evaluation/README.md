# Train Reliability Evaluator

This application allows the evaluation of the overall train reliability given the unreliability of its coaches.
The Train Reliability Evaluator application depends on **librbd**, an RBD evaluation library.

## Building from source

### Prerequisites

The Train Reliability Evaluator application has been developed, compiled and tested on Linux OS with GCC.
It is recommended to the same OS and compiler, otherwise arrangements have to be performed.

### Building librbd

For this application, it is recommended to build librbd as a static library as follows:
- Clone the [librbd RBD evaluation library](https://github.com/marcopapini/librbd)
- Build it as a static library by following the provided build instructions
> **_NOTE:_** the authors used compiled the librbd using `CPU_SMP=0` and `CPU_SIMD=1` preprocessor symbols

### Building trainReliability

To build the Train Reliability Evaluator application, perform the following operations 
- Copy the two files `source/rbd.h` and `make/librbd.a` contained in librbd repository into the folder `trainReliability/source` of this repository
- Open a terminal from the folder `trainReliability/source` and run the following command:

```sh
gcc -L"./" -pthread -o "trainReliability" main.c -lrbd
```

## Run the Train Reliability Evaluator application

The FaultFlow library, after having performed the pyramis analysis, provides as output a *.csv* file.
Since FaultFlow is used to analyse the unreliability of each coach, then its output is composed of **_N_** output files, one for each of the analysed coaches.

### Train Reliability Evaluation -- 3 coaches

To evaluate the reliability of a train with three coaches, suppose that the unreliability of the three coaches is in files `coach1.csv`, `coach2.csv` and `coach3.csv` and suppose that the file `train.csv` should be filled with the overall train reliability, then to evaluate the reliability run the following command:

```sh
./trainReliability 3 coach1.csv coach2.csv coach3.csv train.csv
```

### Train Reliability Evaluation -- 4 coaches

To evaluate the reliability of a train with four coaches, suppose that the unreliability of the three coaches is in files `coach1.csv`, `coach2.csv`, `coach3.csv` and `coach4.csv` and suppose that the file `train.csv` should be filled with the overall train reliability, then to evaluate the reliability run the following command:

```sh
./trainReliability 4 coach1.csv coach2.csv coach3.csv coach4.csv train.csv
```

### Train Reliability Evaluation -- 5 coaches

To evaluate the reliability of a train with five coaches, suppose that the unreliability of the three coaches is in files `coach1.csv`, `coach2.csv`, `coach3.csv`, `coach4.csv` and `coach5.csv` and suppose that the file `train.csv` should be filled with the overall train reliability, then to evaluate the reliability run the following command:

```sh
./trainReliability 5 coach1.csv coach2.csv coach3.csv coach4.csv coach5.csv train.csv
```

### Train Reliability Evaluation -- 6 coaches

To evaluate the reliability of a train with six coaches, suppose that the unreliability of the three coaches is in files `coach1.csv`, `coach2.csv`, `coach3.csv`, `coach4.csv`, `coach5.csv` and `coach6.csv` and suppose that the file `train.csv` should be filled with the overall train reliability, then to evaluate the reliability run the following command:

```sh
./trainReliability 6 coach1.csv coach2.csv coach3.csv coach4.csv coach5.csv coach6.csv train.csv
```

# License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program. If not, see  [https://www.gnu.org/licenses/](https://www.gnu.org/licenses/).

