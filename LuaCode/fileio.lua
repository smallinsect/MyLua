

--文件I/O
--文件模式 r w a r+ w+ a+ b + 
f = io.open("data.txt", "r")
if f == nil then
    print("文件打开失败")
end
print("文件打开成功")

sz = f:read()
print(sz)


f:close()
