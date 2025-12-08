#ifndef __UTILS_H__
#define __UTILS_H__

#define DBG(expr) \
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " \
              << #expr << " = " << (expr) << std::endl;

#endif