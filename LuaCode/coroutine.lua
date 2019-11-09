
--协同函数

--定义协同函数
co = coroutine.create(
    function(a,b)
        print(a+b)
        print(coroutine.running())
        coroutine.yield(a*b)--暂停
        print(a-b)
        return a*a*b*b
    end
)

--获取协同状态
print(coroutine.status(co))

res1,res2=coroutine.resume( co, 11, 22)
print(res1,res2)
res1,res2=coroutine.resume( co)--第二次启动
print(res1,res2)

--第二种定义协同函数
co1=coroutine.wrap( 
    function (a,b)
        print(a*b)
    end
)

co1(20,30)
--co1(30,30)