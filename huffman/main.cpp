#include <iostream>
#include <fstream>
#include "archiever.h"

using namespace std;

int main(int argc, char *argv[])
{
    try {
        if(argc != 4)
        {
            cout << "Wrong number of arguments";
            return EXIT_FAILURE;
        }

        ifstream input(argv[1]);
        if(!input.good())
        {
            cout << "Incorrect input file name";
            return EXIT_FAILURE;
        }

        ofstream output(argv[2]);
        if(!output.good())
        {
            cout << "Incorrect output file name";
            return EXIT_FAILURE;
        }

        string mode = argv[3];
        if(mode != "-pack" && mode != "-unpack")
        {
            cout << "Wrong mode type";
            return EXIT_FAILURE;
        }

        const size_t SCAN_SIZE = 1 << 15;

        if(mode == "-pack")
        {
            archiever x;
            for(;;)
            {
                char scanned[SCAN_SIZE];
                input.read(scanned, SCAN_SIZE);
                size_t symbols_cnt = input.gcount();

                if(symbols_cnt == 0)
                    break;

                x.update(scanned, symbols_cnt);
            }

            string decoding_info = x.get_info();
            output << decoding_info;

            input.clear();
            input.seekg(0);

            vector <char> to_print;
            for(;;)
            {
                char scanned[SCAN_SIZE];
                input.read(scanned, SCAN_SIZE);
                size_t symbols_cnt = input.gcount();

                if(symbols_cnt == 0)
                    break;

                to_print.clear();
                x.encode(scanned, symbols_cnt, to_print);

                output.write(to_print.data(), to_print.size());
            }
            if(!x.end())
                output << x.get_last();
        } else
        {
            vector <uint64_t> data;
            string s;
            for(size_t i = 0; i != 256; i++)
            {
                getline(input, s);
                data.push_back(my_stoll(s));
            }
            archiever x(data);

            vector <char> to_print;
            for(;;)
            {
                char scanned[SCAN_SIZE];
                input.read(scanned, SCAN_SIZE);
                size_t symbols_cnt = input.gcount();

                if(symbols_cnt == 0)
                    break;

                to_print.clear();
                x.decode(scanned, symbols_cnt, to_print);

                output.write(to_print.data(), to_print.size());
            }
        }
        return EXIT_SUCCESS;
    } catch(const std::exception& a) {
        cout << a.what() << endl;
        return EXIT_FAILURE;
    } catch(...) {
        cout << "Something wrong" << endl;
        return EXIT_FAILURE;
    }
}
