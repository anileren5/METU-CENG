# CENG495 HW3

## Development Environment Properties
- Operating system: macOS Sonoma 14.3.1

- Hadoop release: 3.3.6

- JDK Version: 1.8.0_201

- IDE: Visual Studio

## Project structure

- input : This is the folder including the csv and tsv version of the input and python script that is needed to convert csv file to tsv file.

- output: This is the folder including outputs of my code.

- src: This is the folder including the source code.

## How to create preprocessed data ?
I preffered to use the input data (salaries.csv) in the form of tsv. Therefore, I wrote a script (preprocess_data.py). 

You can create salaries.tsv by the following command:

```
cd input
python3 preprocess_data.py
```

Note: Make sure that preprocess_data.py and salaries.csv are in the same directory.

This command will create salaries.tsv in the same directory as salaries.csv. This tsv file will be the input to my map-reduce code.

## How to locate the input file in HDFS ?
You can add the salaries.tsv to HDFS by the following command:

```
cd input
hadoop fs -put ./salaries.tsv /salaries.tsv 
```

## How to compile the source code ?
Source code is under the src folder. You can compile the source code by the following command:

```
cd src
hadoop com.sun.tools.javac.Main *.java
jar cf Hw3.jar *.class     
```

## How to run ?

You can run cases by the following commands:

```
cd src
hadoop jar Hw3.jar total /salaries.tsv /output_total     
hadoop jar Hw3.jar jobtitle /salaries.tsv /output_jobtitle    
hadoop jar Hw3.jar titleexperience /salaries.tsv /output_titleexperience
hadoop jar Hw3.jar employeeresidence /salaries.tsv /output_employeeresidence
hadoop jar Hw3.jar averageyear /salaries.tsv /output_averageyear
```

Note: Make sure that you are in the src directory to run these commands since it is directory including the jar file Hw3.jar.

