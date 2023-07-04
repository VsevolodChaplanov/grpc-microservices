#include "matrix_server/server.hpp"
#include <algorithm>

MatrixMultiplyerServer::~MatrixMultiplyerServer() {
    thread_pool_.join();
    server_->Shutdown();
}