#include "include/client_impl.h"
#include <grpc++/grpc++.h>
#include <vector>
#include <string>

// this function runs n requests each from a different client in parallel
// each client has a variable 'nr' with a value of 1 if the request was successfully processed or 2 if the request returns an error

void run_n_clients_parallel(int n) {

    std::vector<ClientImpl> arr;
    std::vector<std::thread> t;

    for(int i = 0; i < n; i++)
        arr.push_back(ClientImpl(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())));

    
    for (int i = 0; i < arr.size(); ++i) {
        std::string request{std::to_string(i)};
        std::cout<<i<<" ";
        t.push_back(std::thread(&ClientImpl::ProtoMethod, &arr[i], request));
    }
    std::cout<<std::endl;

    for (auto &t1: t) {
        t1.join();
    }

    int count = 0;

    for(int i = 0; i < arr.size(); i++){
        std::cout<<arr[i].getNr()<<" ";
        if(arr[i].getNr() == 1) count++; 
    }

    std::cout << std::endl << "The nr of clients that send the request successfully is: " << count << std::endl;

}

// this function sends n requests of the same synchronus client to the server

void run_n_requests_from_one_client(int n) {

    ClientImpl client{grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())};

    for (int i = 0; i < n; ++i) {  
        std::string request{std::to_string(i)};
        client.ProtoMethod(request);
    }

}


int main() {

    run_n_clients_parallel(3000);

    //run_n_requests_from_one_client(5);

    return 0;
}