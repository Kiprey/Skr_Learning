# 第1~2周

## 进度

- SGI STL 刚抄完list
- 遇到不懂的就去《STL源码剖析》和各个博客上找

## 思考题

### 思考题1

> 在写容器的时候 容器里有很多的成员函数或者非成员函数 。有的容器都有对元素的增删操作 ，那么这些操作在如何不恰当使用的情况下，会造成安全问题请构造出poc来  。

- vector中， push_back函数会调用construct函数来构造一个新的结构  
  如果结构里没有`拷贝构造函数`，那么construct函数就会调用结构体默认的拷贝构造函数  
  但这有个问题，如果该结构体中有指针，例如以下`POC`:
  > 注：以下两个`POC`均使asan成功输出相应的`log`。
  
    ```cpp
    // double free POC
    #include "stl_vector.h"
    #include <iostream>
    #include <string>

    using namespace std;
    using __STL_NAME::vector;

    class test
    {
    private:
        int* _ptr;
        string _id;
    public:
        test(string id = "-1")
        {
            _id = id;
            _ptr = new int(0x1000);
            cout << _id << " constructed." << endl;
        }

        ~test()
        {
            cout << _id << " destroyed." << endl;
            delete _ptr;
        }
    };

    int main()
    {
        vector<test> test1;
        test1.push_back(test("test"));
        test1.erase(test1.begin());

        return 0;
    }
    ```

  结构体`test`中会对指针进行new和delete操作。在没有拷贝构造函数的情况下，push_back只会将对应的结构体`浅拷贝`。  
  而其中的临时变量`test("test")`在push_back之后便原地析构，delete掉了对应的指针。  
  但vector中结构体的指针却还是指向之前的内存，若执行`erase`，则会造成`double free`。  
  又或者如果继续使用该指针，便会造成`UAF`，例如以下`POC`:

    ```cpp
    #include "stl_vector.h"
    #include <iostream>
    #include <string>

    using namespace std;
    using __STL_NAME::vector;

    class test
    {
    private:
        string* _ptr;
    public:
        test(string id = "-1")
        {
            _ptr = new string[10];
            for (int i = 0; i < 10; i++)
                _ptr[i] = id;
            cout << _ptr[0] << " constructed." << endl;
        }
        void print()
        {
            // UAF
            cout << _ptr[0] << " printed." << endl;
        }
        ~test()
        {
            cout << _ptr[0] << " destroyed." << endl;
            delete[] _ptr;
        }
    };

    int main()
    {
        vector<test> test1;
        test1.push_back(test("test"));
        test1.begin()->print();

        return 0;
    }
    ```

  > 解决方法：声明一个`拷贝构造函数`。
- 还有一个与上一点类似的问题，`erase`函数内部的`copy`会执行`operator =`。  
  如果没有重载`operator =`的话，效果会和上一点一样，只进行浅拷贝，从而产生`UAF` / `double free`。（原理与上一点相同，`POC`略）
  > 解决方法：重载`operator =`。

### 思考题2

> 为什么实现了uninitialized_xxx和copy/fill这样两组不同的函数

- `uninitialized_xx`函数，其内部会执行结构体的构造函数。该函数面向于`尚未进行初始化`的内存。
- 而没有“`uninitialized_`”前缀的函数，面向的是`已经初始化过`的内存。所以不需要再执行构造函数。

### 思考题3

> 理解每个容器的内存模型。

- vector的内存模型是一个数组。
- list的内存模型是一个双向链表，由多个节点组成。

## 结题小测试

> 可复制版本：https://paste.ubuntu.com/p/WhyFgV78Qt/

```cpp
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;
class Element {
private:
    int number;
public:
    Element() :number(0) {
        cout << "ctor" << endl;
    }
    Element(int num) :number(num) {
        cout << "ctor" << endl;
    }
    Element(const Element& e) :number(e.number) {
        cout << "copy ctor" << endl;
    }
    Element(Element&& e) :number(e.number) {
        cout << "right value ctor" << endl;
    }
    ~Element() {
        cout << "dtor" << endl;
    }
    void operator=(const Element& item) {
        number = item.number;
    }
    bool operator==(const Element& item) {
        return (number == item.number);
    }
    void operator()() {
        cout << number;
    }
    int GetNumber() {
        return number;
    }
};
template<typename T>
class Vector {
private:
    T* items;
    int count;
public:
    Vector() :count{ 0 }, items{ nullptr } {

    }
    Vector(const Vector& vector) :count{ vector.count } {
        items = static_cast<T*>(malloc(sizeof(T) * count));
        memcpy(items, vector.items, sizeof(T) * count);
    }
    Vector(Vector&& vector) :count{ vector.count }, items{ vector.items } {
        //TODO
        vector.items = nullptr;
        vector.count = 0;
    }
    ~Vector() {
        //TODO
        Clear();
    }
    T& operator[](int index) {
        if (index < 0 || index >= count) {
            cout << "invalid index" << endl;
            return items[0];
        }
        return items[index];
    }
    int returnCount() {
        return count;
    }
    void Clear() {
        //TODO
        for (int i = 0; i < count; i++)
            items[i].~T();
        free(items);
        items = nullptr;
        count = 0;
    }

    void Add(const T& item) {
        //TODO
        Insert(item, count);
    }
    bool Insert(const T& item, int index) {
        //TODO
        if (index < 0 || index > count)
            return false;

        int _count = count + 1;
        T* _items = (T*)malloc(sizeof(T) * _count);

        for (int i = 0; i < index; i++)
            new (_items + i) T(move(items[i]));
        new (_items + index) T(item);
        for (int i = index; i < count; i++)
            new (_items + i + 1) T(move(items[i]));

        Clear();
        items = _items;
        count = _count;
        return true;
    }
    bool Remove(int index) {
        //TODO
        if (index < 0 || index >= count)
            return false;

        int _count = count - 1;
        T* _items = (T*)malloc(sizeof(T) * _count);
        for (int i = 0; i < index; i++)
            new (_items + i) T(move(items[i]));
        for (int i = index + 1; i < count; i++)
            new (_items + i - 1) T(move(items[i]));

        Clear();
        items = _items;
        count = _count;

        return true;
    }
    int Contains(const T& item) {
        //TODO
        for (int i = 0; i < count; i++)
            if (items[i] == item)
                return i;
        return -1;
    }
};
template<typename T>
void PrintVector(Vector<T>& v) {
    int count = v.returnCount();
    for (int i = 0; i < count; i++)
    {
        v[i]();
        cout << " ";
    }
    cout << endl;
}
int main() {
    Vector<Element>v;
    for (int i = 0; i < 4; i++) {
        Element e(i);
        v.Add(e);
    }
    PrintVector(v);
    Element e2(4);
    if (!v.Insert(e2, 10))
    {
        v.Insert(e2, 2);
    }
    PrintVector(v);
    if (!v.Remove(10))
    {
        v.Remove(2);
    }
    PrintVector(v);
    Element e3(1), e4(10);
    cout << v.Contains(e3) << endl;
    cout << v.Contains(e4) << endl;
    Vector<Element>v2(v);
    Vector<Element>v3(move(v2));
    PrintVector(v3);
    v2.Add(e3);
    PrintVector(v2);
    return 0;
}
```
