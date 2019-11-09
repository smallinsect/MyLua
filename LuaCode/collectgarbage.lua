--垃圾回收机制

--垃圾回收函数

--当前垃圾所占内存大小
print(collectgarbage("count"))
mytable={"哈哈哈","嘿嘿嘿","呵呵呵"}
print(collectgarbage("count"))
--强制的执行一次垃圾回收
collectgarbage("collect")
print(collectgarbage("count"))
