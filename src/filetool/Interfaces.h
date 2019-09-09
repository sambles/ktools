#ifndef KTOOLS_FILETOOL_H_
#define KTOOLS_FILETOOL_H_

namespace ktools { namespace filetool {

class OutputRenderer
{
public:
    void render(void *data);
};

class FileReader
{
public:
    void read(const std::string& prefix, OutputRenderer& renderer);
};

}}

#endif
