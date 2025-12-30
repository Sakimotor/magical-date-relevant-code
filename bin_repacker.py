import os
import sys
import re
# a .TIM file's naming, when exported from tim2view is the following: 
# given a STUFF.BIN file, if we extract the .TIM file number 17, with a BPP of 04, it will be called STUFF_000017_04b.tim



def replace(archive_name: str):
    
   #create a variable that will store the binary data we want to write, in chunks
    
   #just like in C, if we manage to open the file, we do something with it. "rb" means read binary data 
    with open(archive_name, "rb") as archive:
        #we read our file and interpret its' data for personal use (see last pin for explanation): 
        # here we read the first 4 bytes in little endian to know how many .TIM files the archive contains
        buffer = archive.read(4)
        files_amount = int.from_bytes( buffer, 'little') + 1
        
        file_chunks = [None]*files_amount
        #we  create a list that will contain the addresses of all the .TIM files
        files_positions = []
        files_sizes = []
        
        
        for i in range(files_amount):
            #now that we know how many files the archive contains, we know how many addresses the header contains
            buffer = archive.read(4)
            buffer_to_int = int.from_bytes(buffer, 'little', signed=False)    
            
            #default case, header is at position 0 and tells its size immediatly
            if i == 0:     
                files_positions.append(0) 
                files_sizes.append(buffer_to_int)
            else:
                files_sizes.append(buffer_to_int - files_sizes[i-1] - files_positions[i-1])
                files_positions.append(files_positions[i-1] + files_sizes[i-1])


        #the final file in the archive goes from the final offset to the end of the file
        archive.seek(files_positions[files_amount - 1], 0)
        files_sizes[files_amount - 1] = len(archive.read())
           
        #we make sure to compare the lowercase values so that .tim, .tIm and .TIM would match alike
        #we also make sure that the .TIM contains the .BIN file's name, without the extension (thus [:-4])
        cur_tims = [x for x in os.listdir() if '.tim' in x.lower() and archive_name[:-4].lower() in x.lower()]
        

        
        
        matched_tims = [None]*files_amount #allocate array out of convenience
        for file in cur_tims:
            split = file.split("_") #split string into an array
            matched_tims[int(split[1])] = file #get the numbered value of the TIM file from the name, 
            #and store the name in that position into the array
           
        
        #we put our reading cursor a the beginning of the archive once again
        i = 0
        archive.seek(0, 0)
		
        for file in range(files_amount):
            #we read data chunks and, if some files are to be replaced, we do
            if i == 0:
                #we skip our header here
                header_len = (files_amount)*4
                files_positions[i] = 0
                archive.read(header_len)
            else:
                files_positions[i] = files_positions[i-1] + files_sizes[i-1]
                #if a .tim file is in our current folder, we set the chunk to it    
                if matched_tims[i] is not None:
                    print(matched_tims[i])
                    with open(matched_tims[i], "rb") as tim_cur:
                        buffer = tim_cur.read()
                        archive.seek(files_sizes[i], 1)
                        files_sizes[i] = len(buffer)  
                else: #otherwise we just read the original file's data
                    buffer = archive.read(files_sizes[i])
            file_chunks[i] = buffer
            i += 1
        #we prepare our new header file with the correct offsets
        buffer = (files_amount - 1).to_bytes(4, 'little', signed=False)
        for j in range(1, files_amount):
            #for each file created previously, we write its position in the header
            buffer += files_positions[j].to_bytes(4, 'little', signed=False)
            print(f"buffer File {j}: position {hex(files_positions[j])}, size {hex(files_sizes[j])} ") 
        file_chunks[0] = buffer
        print(file_chunks[0])
        for i in range(files_amount):
            print(f"files_amount File {i}: position {hex(files_positions[i])}, size {hex(files_sizes[i])} ") 
                    
        #we merge all the chunks in a new file
        with open(f"{archive_name[:-4]}_new.BIN", "wb") as newarc:
            for chunk in file_chunks:
                newarc.write(chunk)
                    
                
            
        
            
        
        
        
        
if __name__ == "__main__":
    # Check if filename is provided as an argument
    if len(sys.argv) != 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    replace(filename)
    
    
    