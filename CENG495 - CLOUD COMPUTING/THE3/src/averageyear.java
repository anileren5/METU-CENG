import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class averageyear {

    // Mapper<KEYIN,VALUEIN,KEYOUT,VALUEOUT>
    public static class AverageyearMapper extends Mapper<LongWritable, Text, IntWritable, DoubleWritable> {

            public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
                // Skip the header line of the input data
                if (key.get() == 0 && value.toString().contains("work_year")) return ;

                // Split the row into columns
                String[] columns = value.toString().split("\t");

                // I have checked the input data and it has 11 columns for each row, so there is no need for additional check 
                double salary = Double.parseDouble(columns[6]);
                int work_year = Integer.parseInt(columns[0]);
                context.write(new IntWritable(work_year), new DoubleWritable(salary));
                return ;
            }
    
    }

    public static class AverageyearPartitioner extends Partitioner<IntWritable, DoubleWritable> {

        public int getPartition(IntWritable key, DoubleWritable value, int numPartitions) {
            // Ensure numPartitions is at least 3
            numPartitions = Math.max(numPartitions, 3);

            int keyValue = key.get();

            // Partition based on the key value
            if (keyValue == 2024) {
                // Send key 2024 to partition 0
                return 0;
            } else if (keyValue == 2023) {
                // Send key 2023 to partition 1
                return 1;
            } else {
                // Send all other keys to partition 2
                return 2;
            }
        }

    }

    // Reducer<KEYIN,VALUEIN,KEYOUT,VALUEOUT>
    public static class AverageyearReducer extends Reducer<IntWritable, DoubleWritable, IntWritable, DoubleWritable> {

        public void reduce(IntWritable key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {
            double total_salary = 0;
            int number_of_values = 0;
            for (DoubleWritable value : values) {
                total_salary += value.get();
                number_of_values += 1;
            }
            double average_salary = total_salary / number_of_values;
            context.write(key, new DoubleWritable(average_salary));
            return ;
        }

    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "averageyear");
        job.setJarByClass(averageyear.class);
        job.setMapperClass(AverageyearMapper.class);
        job.setPartitionerClass(AverageyearPartitioner.class);
        job.setReducerClass(AverageyearReducer.class);
        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(DoubleWritable.class);
        job.setNumReduceTasks(3);
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}