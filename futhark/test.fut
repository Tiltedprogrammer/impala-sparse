import "sparse"

let csrA = {
            values = [3.0, 7.0, 1.0, 2.0, 1.0, 4.0, 5.0],
	    columns = [0i64,2i64,3i64,2i64,1i64,2i64,3i64],
	    offsets = [0i64,3i64,4i64,7i64]
	   }

let csrB = {
            values = [4.0,1.0,5.0,3.0,1.0,4.0],
	    columns = [1i64,2i64,0i64,3i64,0i64,1i64],
	    offsets = [0i64,2i64,4i64,6i64]
	   }

-- 3x4 matrix + * and min plus

-- + *

let plusTimesSemiring : semiring f64 = {add = (+), mult = (*), zero = 0.0}

let csrIntermediate =  (eWiseMult csrA csrB plusTimesSemiring)
    
let csrC =
  let csrIntermediate =  (eWiseAdd csrA csrB plusTimesSemiring)
  let sizeLeft = length csrIntermediate.values
  let sizeRight = length csrA.values
  let rows = length csrIntermediate.offsets
  in
  eWiseMult (csrIntermediate :> csr f64 [sizeLeft] [rows])
	   (csrA :> csr f64 [sizeRight] [rows]) plusTimesSemiring

--let minPlusSemiring
