#ifndef INLINER_HPP
    #define INLINER_HPP

    #include <string>
    #include <vector>

    class FileInliner {
        public:
            FileInliner(std::string source_file_path, std::string result_file_path, unsigned int tab_spaces);
            bool verify_source(std::string* error) const;
            void inline_file();
            void store_result() const;
        private:
            std::string file_path, result_file;
            std::string file_inlined;
            unsigned int tab_spaces;
    };

    class FileListInliner {
        public:
            FileListInliner(std::string file_list_path, std::string result_folder_path, unsigned int tab_spaces);
            bool verify_sources(std::string* error);
            void inline_files() const;
            void store_results() const;
            void clear();
        private:
            std::string file_list_path, result_folder_path;
            unsigned int tab_spaces;
            std::vector<FileInliner*> inlined_files;
            std::vector<std::string> files_folder;
            void create_folder() const;
    };

#endif