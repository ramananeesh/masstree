#include "masstree.c"
#include <fstream>
#include <iostream>

using namespace std;
template <typename key_type> std::vector<key_type> read_bin(const char *filename)
{
    fstream inputFile;
    inputFile.seekg(0, std::ios::end);
    const std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    std::cout << "going to read size = " << fileSize / sizeof(key_type) << endl;
    std::vector<key_type> data(fileSize / sizeof(key_type));
    inputFile.read(reinterpret_cast<char *>(data.data()), fileSize);
    return data;
}

int main(char **argv, int argc)
{
    bool verbose = false;
    int N;
    string input_file;

    // read input arguments
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " -n <number of keys> -i <input file> [-v]" << endl;
        return 1;
    }
    for (int i = 1; i < argc;)
    {
        if (string(argv[i]) == "-n")
        {
            N = stoi(argv[i + 1]);
            i += 2;
        }
        else if (string(argv[i]) == "-i")
        {
            input_file = argv[i + 1];
            i += 2;
        }
        else if (string(argv[i]) == "-v")
        {
            verbose = true;
            i++;
        }
        else if (string(argv[i]) == "-h")
        {
            cout << "Usage: " << argv[0] << " -n <number of keys> -i <input file> [-v]" << endl;
            return 0;
        }
        else
        {
            cout << "Usage: " << argv[0] << " -n <number of keys> -i <input file> [-v]" << endl;
            return 1;
        }
    }

    // read input file
    auto keys = read_bin<uint>(input_file.c_str());
    if (verbose)
        cout << "Read " << keys.size() << " keys from " << input_file << endl;

    if (verbose)
        cout << "N = " << N << endl;

    // initialize masstree
    masstree_t *tree = masstree_create(NULL);

    if (verbose)
        cout << "Starting insertions" << endl;

    // insert keys
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    {
        uint key = keys[i];
        masstree_put(tree, &key, sizeof(uint), (void *)0x1);
    }
    auto end = chrono::high_resolution_clock::now();

    // calculate duration in nanoseconds
    auto insert_duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    // print insert duration
    if (verbose)
        cout << "Insertion duration: " << insert_duration << " ns" << endl;

    if (verbose)
        cout << "Starting lookups" << endl;

    // lookups
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
    {
        uint key = keys[i];
        void *val = masstree_get(tree, &key, sizeof(uint));
    }
    end = chrono::high_resolution_clock::now();

    // calculate duration in nanoseconds
    auto lookup_duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    // print lookup duration
    if (verbose)
        cout << "Lookup duration: " << lookup_duration << " ns" << endl;

    if (verbose)
        cout << "Destroying tree" << endl;
    // destroy masstree
    masstree_destroy(tree);

    cout << insert_duration << ", " << lookup_duration << endl;

    return 0;
}