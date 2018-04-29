(module
 (type $0 (func (param f64 f64) (result f64)))
 (import "env" "__linear_memory" (memory $0 0))
 (import "env" "__indirect_function_table" (table 0 anyfunc))
 (export "average" (func $average))
 (func $average (type $0) (param $var$0 f64) (param $var$1 f64) (result f64)
  (f64.mul
   (f64.add
    (get_local $var$0)
    (get_local $var$1)
   )
   (f64.const 0.5)
  )
 )
 ;; custom section "linking", size 0
)

