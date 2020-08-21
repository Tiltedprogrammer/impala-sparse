import os
import sys




if __name__ == "__main__":
    sourse_path = sys.argv[1]
    dst_path = sys.argv[2]

    if os.path.isdir(sourse_path) and os.path.isdir(dst_path):
        for filename in os.listdir(sourse_path):
            with open(os.path.join(sourse_path,filename),'rt') as sf:
                out_file = os.path.join(dst_path, f"${filename}-out")
                if os.path.exists(out_file):
                    os.remove(out_file)
                with open(out_file,'wt') as df:
                    for line in sf:
	#read replace the string and write to output file
	                    df.write(line.replace('integer', 'real'))

    else:
        print("Path to the directory should be specified")