## HTTP篇
### 1.HTTP 缓存有哪些实现方式？
HTTP 缓存有两种实现方式，分别是强制缓存和协商缓存。
#### 什么是强制缓存？
- 强制缓存指的是只要浏览器判断缓存没有过期，就直接用浏览器的本地缓存，决定是否使用缓存的**主动性在浏览器**这里。
- 强缓存是利用下面两个HTTP相应头部（Response Header）字段实现的，它们用来表示资源在客户端的有效期
1. **Cache-Control** 是一个相对时间
2. **Expires** 是一个绝对时间
如果同时有这两个字段的话，**Cache-Control** 字段的优先级较高

#### 什么是协商缓存？
- 304：服务器告诉浏览器可以使用本地缓存的资源
- 协商缓存就是与服务端协商之后，通过协商结果来判断是否使用本地缓存。
- 协商缓存可以基于两种头部来实现
1. 请求头部的**If-Modified-Since** 配合相应头部的 **Last_Modified** 
2. 请求头部中的 **If-None-Match** 字段与响应头部中的 **ETag** 字段（唯一标识相应资源），

### [HTTP各版本和HTTPS](https://xiaolincoding.com/network/2_http/http_interview.html#http-%E7%89%B9%E6%80%A7)