#ifndef INLINER_HPP
    #define INLINER_HPP

    #include <string>

    class Inliner {
        public:
            Inliner(std::string source_file_path, std::string result_file_path, unsigned int tab_spaces);
            bool verify_source() const;
            void inline_file();
            void store_result() const;
        private:
            std::string file_path, result_file;
            std::string file_inlined;
            unsigned int tab_spaces;
    };

#endif