import "lib/github.com/diku-dk/sorts/radix_sort"

local let sort = (\x -> radix_sort i64.num_bits
				   i64.get_bit x)
local let sort_triplets = (\x -> radix_sort_by_key (\(e1,_,_) -> e1)
						   i64.num_bits i64.get_bit x)

--if sort is enough polymorphic, i.e. may have the same name for distinct types,
-- index will be allowed to change almost arbitrary
type index = i64			       



-- if using modules then there will be  sparse functions per each semiring,
-- i.e. in case eWiseAdd, eWiseMult there will be 4 functions for 2 semirings
-- each with a definition like:
-- module sparse (S : semiring) = {
--  let eWiseAdd ...
--  let eWiseMult ...
-- }
--
--module type semiring = {
--  type t
--  val add : t -> t -> t
--  val mult : t-> t -> t
--  val zero : t
--}

type csr 't [n] [m] = {values:  [n]t, columns: [n]index, offsets:[m]index}

--non-lifted type abbreviations may not contain functions
type^ semiring 't = {add : t->t->t, mult : t->t->t, zero : t}

local let a : csr i32 [3] [4] = {values = [4,2,3], columns = [0,1,2], offsets = [0,1,2,3]}
								     
--union and intersection of columns of 2 rows of 2 matrices
local let unionSize [n] [m] (a : [n]index) (b : [m]index) : index =
  if null a then length b else
  if null b then length a else
  let ab = sort (a ++ b)
--length at least 2
  let zipped = zip ab (indices ab) in
  (reduce (\(acc,bot) (e,i) -> if (e == zipped[i-1].0) then (acc,bot) else (acc + 1,bot)) (1,1) zipped[1:]).0

local let unionArrays [n] [m](a : [n]index) (b : [m]index) : [](index,i64,(i64,i64)) =
  let b' = zip3 b (replicate m 1i64) (zip (indices b) (replicate m 0i64))
  in
  if null a then b' else
  let a' = zip3 a (replicate n 0i64) (zip (indices a) (replicate n 0i64))
  in
  if null b then a' else
-- ab :: [(index,i64,(i64,i64))] ~ [value,array_id,(array_id's index,optional)]  
  let ab = sort_triplets (a' ++ b')
--length at least 2
  -- zipped :: [(value,array_id,(array_id's index,optional),index)]
  let zipped = zip ab (indices ab)
  let zipped_size = n + m in
  filter (\(x,_,_) -> x != -1) (map (\(e,i) -> 
					 if i != (zipped_size - 1) &&
						     (e.0 == (zipped[i+1].0).0)
					 then (-1i64,0i64,(0i64,0i64))
					 else if i != 0 &&
						 (e.0 == (zipped[i-1].0).0) then
					      if e.1 == 0 then
						(e.0,2,((e.2).0,((zipped[i-1].0).2).0))
					      else
						(e.0,2,(((zipped[i-1].0).2).0,(e.2).0))
					 else e) zipped)  

--due to the fact that we need the intersection of
 --indices, there might be only 2 consequtive equal indeces after sorting	   

local let intersectSize [n] [m](a : [n]index) (b : [m]index) : index =
  if null a then 0 else
  if null b then 0 else
  let ab = sort (a ++ b)
--length at least 2
  let zipped = zip ab (indices ab) in
  (reduce (\(acc,bot) (e,i) -> if (e == zipped[i-1].0) then (acc + 1,bot) else (acc,bot)) (0,1) zipped[1:]).0

local let intersectArrays [n] [m] (a : [n]index) (b : [m]index) : [](index,(i64,i64)) =
  if null a then [] else
  if null b then [] else
  let a' = zip3 a (replicate n 0i64) (indices a)
  let b' = zip3 b (replicate m 1i64) (indices b) in
  let ab = sort_triplets (a' ++ b')
  --length at least 2
  --zipped :: [(value,array_id,array_id's_index),index]
  let zipped = zip ab (indices ab) in
  filter (\(x,_) -> x != -1) (map (\((value,array_id,array_id's_index),i) ->
				 if value == (zipped[i-1].0).0
					      then
						if array_id == 0
						then (value,(array_id's_index,
							     (zipped[i-1].0).2))
						else (value,((zipped[i-1].0).2,
							     array_id's_index
							     ))
						  
					 else (-1,(0,0))) zipped[1:])  

--module sparse (S : semiring) = {

--now we can compute the size of the resulting matrix
--assume non-empty
local let eWiseAddOffsets 't [n] [k] [m] (left : csr t [n] [m])
                             (right : csr t [k] [m]) : ([m]index,i64) =
  let offsets =
    scan (+) (0) (map2 (\(x1,x2) (y1,y2) -> let prevx2 = if x2 == 0 then x2 else x2 - 1
			    let prevy2 = if y2 == 0 then y2 else y2 - 1 in
			    (unionSize left.columns[left.offsets[prevx2]:x1]
					   right.columns[right.offsets[prevy2]:y1]))
       (zip left.offsets (indices left.offsets))
       (zip right.offsets (indices right.offsets))
	       ) in (offsets,offsets[m-1])

local let eWiseMultOffsets 't [n] [k] [m] (left : csr t [n] [m])
                             (right : csr t [k] [m]) :  ([m]index,i64) =
  let offsets =
    scan (+) (0) (map2 (\(x1,x2) (y1,y2) -> let prevx2 = if x2 == 0 then x2 else x2 - 1
			    let prevy2 = if y2 == 0 then y2 else y2 - 1 in
			    (intersectSize left.columns[left.offsets[prevx2]:x1]
					   right.columns[right.offsets[prevy2]:y1]))
       (zip left.offsets (indices left.offsets))
       (zip right.offsets (indices right.offsets))
	   ) in (offsets,offsets[m-1])

let eWiseAdd 't [n] [k] [m] (left : csr t [n] [m])
                         (right : csr t [k] [m]) (s : semiring t) : csr t [] [m] =

  let (offsets,nnz) = eWiseAddOffsets left right
  let values = replicate nnz s.zero
  let columns = replicate nnz (0 :> index)
  let leftOffsets = zip left.offsets (indices left.offsets)  
  let rightOffsets = zip right.offsets (indices right.offsets)
  let (columns',values') = loop (columns',values') = (columns,values) for i < m-1
			   do(
    let (new_columns,array_id,array_id's_index) = unzip3 (unionArrays
			       left.columns[left.offsets[i]:left.offsets[i+1]]
			       right.columns[right.offsets[i]:right.offsets[i+1]])
    let indxs = offsets[i+1] - offsets[i]
    let row = (([offsets[i]..<offsets[i+1]])[0] :> [indxs]i64)
    let row_left = left.offsets[i]
    let row_right = right.offsets[i]
			   in
  (scatter columns' row (new_columns :>[indxs]index),
   scatter values' row ((map (\(_,j) ->
			         if array_id[j] == 2
			         then (s.add left.values[row_left + array_id's_index[j].0]
				      right.values[row_right + array_id's_index[j].1])
			         else if array_id[j] == 0
				 then left.values[row_left + array_id's_index[j].0]
				 else right.values[row_right + array_id's_index[j].0])
			(zip new_columns (indices new_columns))) :>[indxs]t)
		       )
  )
  in {values = values', columns = columns',offsets = offsets}

let eWiseMult 't [n] [k] [m] (left : csr t [n] [m])
                          (right : csr t [k] [m]) (s : semiring t) : csr t [] [m] =

  let (offsets,nnz) = eWiseMultOffsets left right
  let values = replicate nnz s.zero
  let columns = replicate nnz 0
  let leftOffsets = zip left.offsets (indices left.offsets)  
  let rightOffsets = zip right.offsets (indices right.offsets)
  let (columns',values') = loop (columns',values') = (columns,values) for i < m-1
			   do(
    let (new_columns,array_id's_index) = unzip (intersectArrays
			       left.columns[left.offsets[i]:left.offsets[i+1]]
			       right.columns[right.offsets[i]:right.offsets[i+1]])
    let indxs = offsets[i+1] - offsets[i]
    let row = (([offsets[i]..<offsets[i+1]])[0] :> [indxs]index)
    let row_left = left.offsets[i]
    let row_right = right.offsets[i]
			   in
  (scatter columns' row (new_columns :>[indxs]index),
   scatter values' row ((map (\(_,j) -> s.mult
			        left.values[row_left + array_id's_index[j].0]
				right.values[row_right + array_id's_index[j].1])
			(zip new_columns (indices new_columns))) :>[indxs]t)
		       )
  )
  in {values = values', columns = columns',offsets = offsets}


--for testing purposes

-- 2 4 0
-- 0 1 3
-- 0 0 2
local let csrA = {values = [2,4,1,3,2], columns = [0i64,1i64,1i64,2i64,2i64], offsets = [0i64,2i64,4i64,5i64]}

-- 1 5 2
-- 1 0 0
-- 1 0 0
local let csrB = {values = [1,5,2,1,1], columns = [0i64,1i64,2i64,0i64,0i64], offsets = [0i64,3i64,4i64,5i64]}

-- eWiseAdd csrA csrB
-- 3 9 2
-- 1 1 3
-- 1 0 2

-- eWiseMult csrA csrB
-- 2 20 0
-- 0 0 0
-- 0 0 0
let PlusTimesSemiring : semiring f64 = {add = (+), mult = (*), zero = 0.0}

entry eWiseAddEntry [n] [k] [m] (leftValues : [n]f64)
		                (leftColumns : [n]i64)
		                (leftOffsets : [m]i64)
                                (rightValues : [k]f64)
		                (rightColumns : [k]i64)
		                (rightOffsets : [m]i64)
				 : ([]f64,[]i64,[m]i64) =
  let csrLeft = {values = leftValues, columns = leftColumns, offsets = leftOffsets}
  let csrRight = {values = rightValues, columns = rightColumns, offsets = rightOffsets}
  let result = (eWiseAdd csrLeft csrRight PlusTimesSemiring) in
   (result.values,result.columns,result.offsets)

entry eWiseMultEntry [n] [k] [m] (leftValues : [n]f64)
		                (leftColumns : [n]i64)
		                (leftOffsets : [m]i64)
                                (rightValues : [k]f64)
		                (rightColumns : [k]i64)
		                (rightOffsets : [m]i64)
				 : ([]f64,[]i64,[m]i64) =
  let csrLeft = {values = leftValues, columns = leftColumns, offsets = leftOffsets}
  let csrRight = {values = rightValues, columns = rightColumns, offsets = rightOffsets}
  let result = (eWiseMult csrLeft csrRight PlusTimesSemiring) in
   (result.values,result.columns,result.offsets)

entry eWiseAddMultEntry [n] [k] [m] (leftValues : [n]f64)
		                (leftColumns : [n]i64)
		                (leftOffsets : [m]i64)
                                (rightValues : [k]f64)
		                (rightColumns : [k]i64)
		                (rightOffsets : [m]i64)
				 : ([]f64,[]i64,[m]i64) =
  let csrLeft = {values = leftValues, columns = leftColumns, offsets = leftOffsets}
  let csrRight = {values = rightValues, columns = rightColumns, offsets = rightOffsets}
  let resultAdd = (eWiseAdd csrLeft csrRight PlusTimesSemiring)
  let resultSize = length resultAdd.values
  let resultMult = (eWiseMult (resultAdd :> csr f64 [resultSize] [m]) csrRight PlusTimesSemiring) in
  
   (resultMult.values,resultMult.columns,resultMult.offsets)

