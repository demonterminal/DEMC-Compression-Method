# DEMC-Compression-Method
DEMC is a custom compression method used a lot by Demon Project and FOZ DTX for making the files always accessible and it can be directly used with C++ program or CLI of Demon Project

It can also bbe used for compressing unlimited file. (.demc is written purely in C++)

# C++ Tutorial
DEMC_Manager.h : main header file to be installed and included in program with <code>#include</code>
After including file you need to make an instance of the DEMC_Manager class by <code>DEMC_Manager your_instance;</code> eg <code>DEMC_Manager demcManager;</code>

## Methods
## compressFile() -> takes tow args 1 is the file output name after compression and 2nd is the vector array of string indicating the file list to bbe compressed eg <code>std::vector<std::string> files = {"your file", "your file", "your file"};</code> and then finally compress it with compressFile() instance eg:
<pre><code>
    DEMC_Manager demcManager;

    // Compress files
    std::vector<std::string> files = { "a.txt", "b.txt" };
    demcManager.compressFiles("archive.demc", files);
</code></pre>

for error checking compressFile() will return 1 if everything is done perfectly or 0 if any error occurs.
eg

<pre>
  <code>
    DEMC_Manager demcManager;

    // Compress files
    std::vector<std::string> files = { "a.txt", "b.txt" };
    if (demcManager.compressFiles("archive.demc", files)) {
        std::cout << "Compression successful!" << std::endl;
    } else {
        std::cout << "Compression failed!" << std::endl;
    }
  </code>
</pre>

## decompressFile() -> This method is used to decompress the DEMC file by taking 2 args. 1 is the file name and second is the output directory which will be outputted after decompression ends, It also has exact same error checking like previous methods eg.

<pre>
  <code>
    DEMC_Manager demcManager;
    
    // Decompress files
    if (demcManager.decompressFile("archive.demc", "output_dir")) {
        std::cout << "Decompression successful!" << std::endl;
    } else {
        std::cout << "Decompression failed!" << std::endl;
    }

    return 0;
  </code>
</pre>
