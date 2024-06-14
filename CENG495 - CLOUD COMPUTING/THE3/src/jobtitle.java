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

public class jobtitle {

    // Mapper<KEYIN,VALUEIN,KEYOUT,VALUEOUT>
    public static class JobtitleMapper extends Mapper<LongWritable, Text, Text, DoubleWritable> {

            public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
                // Skip the header line of the input data
                if (key.get() == 0 && value.toString().contains("work_year")) return ;

                // Split the row into columns
                String[] columns = value.toString().split("\t");

                // I have checked the input data and it has 11 columns for each row, so there is no need for additional check 
                String job_title = columns[3];
                double salary = Double.parseDouble(columns[6]);
                context.write(new Text(job_title), new DoubleWritable(salary));
                return ;
            }
    
    }

    // Reducer<KEYIN,VALUEIN,KEYOUT,VALUEOUT>
    public static class JobtitleReducer extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {

        public void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {
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
        Job job = Job.getInstance(conf, "jobtitle");
        job.setJarByClass(jobtitle.class);
        job.setMapperClass(JobtitleMapper.class);
        job.setReducerClass(JobtitleReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(DoubleWritable.class);
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}