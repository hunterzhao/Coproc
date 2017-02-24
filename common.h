#ifndef _COMMON_H_
#define _COMMON_H_
#include <memory>
#define DEFINE_SHARED_PTR(typename) \
        typedef std::shared_ptr<typename> typename##Ptr 
#endif