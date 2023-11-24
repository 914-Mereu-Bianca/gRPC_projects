#include "include/client_impl.h"
#include <grpc++/grpc++.h>
#include <vector>
#include <string>


void my_fct(int i) {
    std::cout<<i<<" ";
    std::this_thread::sleep_for(std::chrono::seconds(3));
}


int main() {

    /*std::vector<ClientImpl> arr;
    std::vector<std::thread> t;

    for(int i = 0; i < 20; i++)
        arr.push_back(ClientImpl(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())));

    
    for (int i = 0; i < arr.size(); ++i) {
        std::string request{std::to_string(i)};
        std::cout<<i<<" ";
        t.push_back(std::thread(&ClientImpl::ProtoMethod, &arr[i], request));
    }
    std::cout<<std::endl;
    //ClientImpl client{grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())};

    //std::thread thread{&ClientImpl::AsyncCompleteRPC, &client};
    
    /*for (int i = 1; i < arr.size(); ++i) {
        std::string request1{std::to_string(i)};
        
        arr[i].ProtoMethod(request1);
    }*/

    /*for (auto &t1: t) {
        t1.join();
    }

    int i;
    for(i = 0; i < arr.size(); i++){
        std::cout<<arr[i].nr<<" ";
    }
    std::cout<<std::endl;

    for(i = arr.size() - 1; i > 0; i--){
        if (arr[i].nr == 1)
            break;
    }
    std::cout<<i<<std::endl;*/


    std::vector<std::thread> t;

    for(int i = 0; i < 4000; i++) {
        t.push_back(std::thread(&my_fct, i));
    }


    for (auto &t1: t) {
        t1.join();
    }


    return 0;
}