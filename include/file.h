#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <string>

namespace file
{
    template <typename T>
    class file_t
    {
    private:
        const std::string filename;

    public:
        file_t(const std::string& filename) : filename(filename) {}

        int get_file (std::vector<T>& buff, int& cache_size, int& data_size)
        {
            std::ifstream file(filename);
            if (!file)
            {
                std::cout << "error\n";
                exit(1);
            }

            file >> cache_size;
            file >> data_size;

            buff.reserve(data_size);

            T tmp = 0;
            while (file >> tmp) buff.push_back(tmp);

            file.close();
            return 0;
        }
    };
}

#endif
