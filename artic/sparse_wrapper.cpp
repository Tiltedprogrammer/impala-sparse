#include "csr_wrapper.h"


extern "C"{

    CSR read_csr_f32(const char* path) {

        auto ifstream = std::ifstream(std::string(path));

        auto csr = CSRWrapper<float>(ifstream);

        return csr.csr_to_struct_deep();
    }

    void free_csr_f32(CSR * csr){
        
        free(csr->values);
        free(csr->cols);
        free(csr->row_index);

    }


    void print_csr_f32(CSR * csr) {

        for(int i = 0; i < csr->nnz; i++){
            std::cout << std::setprecision(10) << csr->values[i] << " ";
        }
        std::cout << std::endl;

        for(int i = 0; i < csr->nnz; i++) {
            std::cout << csr->cols[i] << " ";
        }
        std::cout << std::endl;
        for(int i = 0; i < csr->M + 1; i++) {
            std::cout << csr->row_index[i] << " ";
        }
        std::cout << std::endl;

    }

}
//