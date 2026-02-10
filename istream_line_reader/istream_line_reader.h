#include <istream>
#include <string>

namespace Chena {

class istream_line_reader {

     class iterator : public std::iterator<std::input_iterator_tag, std::string> {
          public:
               using iterator_category = std::input_iterator_tag;
               using value_type = std::string;
               using difference_type = std::ptrdiff_t;
               using pointer = std::string*;
               using reference = std::string&;
               using const_reference = const std::string&;
               using const_pointer = const std::string*;

               iterator() : stream_(nullptr) {}

               iterator(std::istream& stream) : stream_(&stream) {
                    ++(*this);
                //    this->operator++();
               }

               iterator(const iterator& other) : stream_(other.stream_), line_(other.line_) {}
               iterator(iterator&& other) noexcept : stream_(other.stream_), line_(std::move(other.line_)) {
                   other.stream_ = nullptr;
               }

               void swap(iterator& other) noexcept {
                   std::swap(stream_, other.stream_);
                   std::swap(line_, other.line_);
               }

               iterator& operator=(iterator other) {
                   swap(other);
                   return *this;
               }

               const_reference operator*() const {
                   return line_;
               }

               const_pointer operator->() const {
                   return &line_;
               }

               iterator& operator++() {
                   std::getline(*stream_, line_);
                    
                   if (!*stream_) {
                       stream_ = nullptr;
                       line_.clear();
                   }

                   return *this;
               }

               iterator operator++(int) {
                   iterator tmp = *this;
                   ++(*this);
                   return tmp;
               }

               friend bool operator==(const iterator& a, const iterator& b) {
                   return a.stream_ == b.stream_;   
               }

               friend bool operator!=(const iterator& a, const iterator& b) {
                   return !(a == b);
               }

          private:
               std::istream* stream_;
               std::string line_;
     };

public:
     istream_line_reader() = delete;
     istream_line_reader(std::istream& stream) : stream_(&stream) {}

     iterator begin() {
          return iterator(*stream_);
     }

     iterator end() {
          return iterator();
     }

private:
    std::istream* stream_;
};

}