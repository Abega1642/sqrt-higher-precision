#include "../include/FileReader.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <string>

class FileReaderTest : public ::testing::Test {
    protected:
        std::string temp_filename;

        void write_temp_file(const std::string& content) {
            temp_filename = "temp_test_file.txt";
            std::ofstream out(temp_filename, std::ios::binary);
            out << content;
            out.close();
        }

        void remove_temp_file() const {
            std::remove(temp_filename.c_str());
        }

        void TearDown() override {
            remove_temp_file();
        }
};

TEST_F(FileReaderTest, ReadsSmallFile) {
    write_temp_file("Hello, world!");
    const FileReader reader(temp_filename);
    EXPECT_EQ(reader.read_all(), "Hello, world!");
}

TEST_F(FileReaderTest, ReadsEmptyFile) {
    write_temp_file("");
    const FileReader reader(temp_filename);
    EXPECT_EQ(reader.read_all(), "");
}

TEST_F(FileReaderTest, ReadsFileWithNewlines) {
    write_temp_file("Line1\nLine2\nLine3");
    const FileReader reader(temp_filename);
    EXPECT_EQ(reader.read_all(), "Line1\nLine2\nLine3");
}

TEST_F(FileReaderTest, ReadsFileWithSpecialCharacters) {
    write_temp_file("©π∆✓漢字");
    const FileReader reader(temp_filename);
    EXPECT_EQ(reader.read_all(), "©π∆✓漢字");
}

TEST_F(FileReaderTest, ReadsBinaryLikeContent) {
    const std::string binary_data = "abc\0def\0ghi";
    write_temp_file(binary_data);
    const FileReader reader(temp_filename);
    const std::string result = reader.read_all();
    EXPECT_EQ(result.size(), binary_data.size());
    EXPECT_EQ(result, binary_data);
}

TEST(FileReaderStandaloneTest, ThrowsOnMissingFile) {
    const FileReader reader("nonexistent_file.txt");
    EXPECT_THROW(reader.read_all(), std::runtime_error);
}

TEST_F(FileReaderTest, ReadsLargeFile) {
    const std::string large(10'000, 'x');
    write_temp_file(large);
    const FileReader reader(temp_filename);
    EXPECT_EQ(reader.read_all(), large);
}

TEST_F(FileReaderTest, ReadsWhitespaceOnlyFile) {
    write_temp_file("   \n\t  ");
    const FileReader reader(temp_filename);
    EXPECT_EQ(reader.read_all(), "   \n\t  ");
}

TEST_F(FileReaderTest, ReadsLongLineFile) {
    const std::string long_line(2048, 'A');
    write_temp_file(long_line);
    const FileReader reader(temp_filename);
    EXPECT_EQ(reader.read_all(), long_line);
}

TEST_F(FileReaderTest, CanReadMultipleTimesIfReused) {
    write_temp_file("Repeatable content");
    const FileReader reader(temp_filename);
    const std::string first = reader.read_all();
    const std::string second = reader.read_all();
    EXPECT_EQ(first, second);
}
