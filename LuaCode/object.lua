

--面向对象

--table function 实现面向对象

--一个对象 有属性和方法
--简单的对象
Person={}

Person.name = "小昆虫"
Person.age=9999

Person.say=function()
    print(Person.name,Person.age)
end

Person.say()

--
Dog={}
Dog.name="小鸡鸡"
Dog.color="黑色"
function Dog:say()
    print(Dog.name,Dog.color)
end

Dog:say()

--构造方法
Cat ={}
Cat.name="咪咪"
function Cat:new()
    t={}

    setmetatable(t, {__index=self})--如果调用一个属性的时候 如果t中不存在 那么会在__index所指定的table中查找
    return t
end

cat1 = Cat:new()
cat2 = Cat:new()
cat1.name = "吉吉"
print(cat1.name)
print(cat2.name)
