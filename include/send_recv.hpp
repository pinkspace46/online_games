#ifndef SEND_RECV_H
#define SEND_RECV_H

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>

#define SEND(FD, PTR, LEN, FLAGS) ({std::cout << "send at line: " << __LINE__ << ", fd = " << (FD) << ", length = " << (LEN) << std::endl;\
                                  int retval;\
                                  if ((retval = send((FD), (PTR), (LEN), (FLAGS))) != (LEN)) \
                                      perror("send");\
                                  retval;})
#define RECV(FD, PTR, LEN, FLAGS) ({std::cout << "recv at line: " << __LINE__ << ", fd = " << (FD) << ", length = " << (LEN) << std::endl;\
                                  int retval;\
                                  if ((retval = recv((FD), (PTR), (LEN), (FLAGS) | MSG_WAITALL)) != (LEN)) \
                                      perror("recv");\
                                  retval;})
#endif /* SEND_RECV_H */
