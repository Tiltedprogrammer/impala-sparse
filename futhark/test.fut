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

let csrC = eWiseAdd (eWiseAdd csrA csrB plusTimesSemiring) csrA plusTimesSemiring

		--should be
		-- 3 4 8 1
		-- 5 0 2 3
		-- 1 5 4 5
--let minPlusSemiring
