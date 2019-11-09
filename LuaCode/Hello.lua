--1、
--单行注释
--[[多行注释
print("hello ")

]]

--2、标示符
--以一个字母A-Z或a-z或者下划线开头后加上0个或多个字母，下划线，数字（0-9）
--不允许使用特殊字符如@$%来定义标示符

--关键词
--and break do else
--elseif end false for
--function if in local
--nil not or repeat
--return then true until
--while

--3、数据类型
--nil boolean number string function userdata thread table
function func()
end
print(type(a))
print(type(true))
print(type(222))
print(type("hhh"))
print(type(func))

--table
tb = {key1="small",key2="insect", key3="hhh"}
print(tb.key1)
tb.key1=nil--置空 内存释放
print(tb.key1)
print(tb.key2)
print(tb.key3)

--逻辑判断
if a then
    print("1")
end

if a then
    print("2")
elseif b then
    print("3")
end

if a then
    print("4")
else
    print("5")
end

--string
str1 = "smallinsect1"
str2 = "smallinsect2"
--多行字符串
str3=[[
<html>
    <head></head>
    <body>
        <a href="http://www.baidu.com">百度链接</a>
    </body>
</html>
]]
print(str1)
print(#str2)--#取字符串长度
print(str3)


tab1={}--空表
tab2={["key1"]=100,["key2"]="value2",[2]=300,["ke"]="ziyou"}--键值对
print(tab1)
print(tab2.key1)
print(tab2["key2"])
tab3={"apple","orange","pear","grape"}
print(tab3[2])

for key, val in pairs(tab3) do 
    print(key.." "..val)
end

--添加key键值
tab4={}
tab4.key1="www.baidu.com"
tab4["dfada"]="dddd"

print("---------------tab5---------------------")
tab5={"111","222","333","444"}
for key, val in pairs(tab5) do 
    print(key.." "..val)
end
print("----------------------------------------")
--释放表中第二个元素
tab5[2]=nil
for key, val in pairs(tab5) do 
    print(key.." "..val)
end
print("---------------tab5---------------------")
--释放表内存
tab5=nil

print("---------------func(n)---------------------")
--函数
function func(n)
    if n == 1 then
        return n
    end
    return n*func(n-1)
end
print(func(10))
print("---------------func(n)---------------------")
print("---------------testFun(tab, fun)---------------------")
function testFun(tab, fun)
    for key, val in pairs(tab) do
        fun(key, val)
    end
end

tab={key1="val1", key2="val2",key3="val3"}
function f1(k,v)
    print(k..":"..v)
end
function f2(k,v)
    print(k.." "..v)
end
testFun(tab, f1)
testFun(tab, f2)
print("---------------testFun(tab, fun)---------------------")

--thread userdata

--多个变量赋值
a,b,c=1,"aa"
print(a,b,c)

print("---------------while---------------------")
a=1
while a < 10 do 
    print(a)
    a = a + 1
end
print("---------------while---------------------")

--lua 函数可变参数
function varParam(...)
    local arg = {...}
    for k,v in pairs(arg) do
        print(k,v)
    end
end

varParam(11)
varParam(111,222)
varParam(1111,2222,3333)
varParam(11111,22222,33333,44444)

--算术运算符 逻辑运算符

--字符串的处理
str = "My name is six god"
str2 = string.upper( str )
str3 = string.lower( str )
print(str,str2,str3)

--数组
array = {"c","c++","html","java"}
for k,v in pairs(array) do 
    print(k,v)
end
--pairs 迭代table，便利表中所有的key跟value
--ipairs 按照索引从1开始，递增便利，遇到nil停止

print("------------square(st,en)---------------")
--迭代函数
function square(state,control)
    if control > state then
        return nil
    end
    control = control + 1
    return control,control*control
end

for i,j in square,9,0 do 
    print(i,j)
end
print("------------square(st,en)---------------")

print("----------------table引用-----------------------")
tab = {"c","c++","lua"}
tab1 = tab--其实就是引用 指向同一块内存
print(tab[1])
print(tab1[1])
tab1[1] = "java"
print(tab[1])
print(tab1[1])
print("----------------table引用-----------------------")
--table函数
print(table.concat(tab))
print(table.concat(tab," "))
print(table.concat(tab," ",1,2))

--插入表数据
table.insert(tab,"javascript")
table.insert(tab,1,"html")
--移除表数据
-- table.remove(tab)
-- table.remove(tab,2)

--表的排序

print(table.concat(tab," "))
table.sort(tab)
print(table.concat(tab," "))



