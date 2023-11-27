#ifndef _CLIENT_SYNC_IMPL_H_
#define _CLIENT_SYNC_IMPL_H_

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include "../../build/proto_generated/data.grpc.pb.h"

class ClientImpl {

public:

    ClientImpl(std::shared_ptr<grpc::Channel> channel);
            
    void ProtoMethod(const std::string &name);

    inline int getNr() { return nr; }    
       
private:

    std::unique_ptr<data::ProtoService::Stub> _stub;

    // The nr has a value of 1 if the client has sent the request successfully or 2 if it resulted in an error
    int nr;

};

#endif  // _CLIENT_SYNC_IMPL_H_