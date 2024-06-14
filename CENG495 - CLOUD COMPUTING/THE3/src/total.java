import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class total {

    // Mapper<KEYIN,VALUEIN,KEYOUT,VALUEOUT>
    public static class TotalMapper extends Mapper<LongWritable, Text, Text, DoubleWritable> {

            public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
                // Skip the header line of the input data
                if (key.get() == 0 && value.toString().contains("work_year")) return ;

                // Split the row into columns
                String[] columns = value.toString().split("\t");

                // I have checked the input data and it has 11 columns for each row, so there is no need for additional check 
                double salary = Double.parseDouble(columns[6]);
                context.write(new Text("salary"), new DoubleWritable(salary));
                return ;
            }
    
    }

    // Reducer<KEYIN,VALUEIN,KEYOUT,VALUEOUT>
    public static class TotalReducer extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {

        public void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {
            double total_salary = 0;
            for (DoubleWritable value : values) total_salary += value.get();
            context.write(new Text("total_salary"), new DoubleWritable(total_salary));
            return ;
        }

    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "total");
        job.setJarByClass(total.class);
        job.setMapperClass(TotalMapper.class);
        job.setReducerClass(TotalReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(DoubleWritable.class);
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}