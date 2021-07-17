#ifndef SEND_RECV_H
#define SEND_RECV_H

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>

#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

/* beware, these macros evalutate FD and LEN multiple times */

#define SEND(FD, PTR, LEN, FLAGS) ({std::cout << GREEN << "SEND" << RESET << " at line: " << __LINE__ << ", fd = " << (FD) << ", length = " << (LEN);\
                                  int retval;\
                                  if ((retval = send((FD), (PTR), (LEN), (FLAGS))) != (LEN)) \
                                      perror("send");\
                                  std::cout << ", sent " << retval << " bytes" << std::endl;\
                                  retval;})
#define RECV(FD, PTR, LEN, FLAGS) ({std::cout << BLUE << "RECV" << RESET << " at line: " << __LINE__ << ", fd = " << (FD) << ", length = " << (LEN);\
                                  int retval;\
                                  retval = recv((FD), (PTR), (LEN), (FLAGS));\
                                  std::cout << ", received " << retval << " bytes" << std::endl;\
                                  retval;})
#endif /* SEND_RECV_H */
