import csv

# Define the input and output file paths
input_csv_file = 'salaries.csv'
output_tsv_file = 'salaries.tsv'

# Open the CSV file for reading and the TSV file for writing
with open(input_csv_file, mode='r', newline='') as csv_file:
    with open(output_tsv_file, mode='w', newline='') as tsv_file:
        # Create CSV reader and TSV writer objects
        csv_reader = csv.reader(csv_file)
        tsv_writer = csv.writer(tsv_file, delimiter='\t')

        # Write each row from the CSV file to the TSV file
        for row in csv_reader:
            tsv_writer.writerow(row)
