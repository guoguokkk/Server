@echo off

::服务端IP地址
::set cmd="strIP=192.168.153.129"
set cmd="strIP=fe80::61dd:873a:c1a0:ab7c%8"

::服务端端口
set cmd=%cmd% nPort=8099

::工作线程数量
set cmd=%cmd% nThread=1

::每个工作线程，创建多少个客户端
set cmd=%cmd% nClient=1

::::::数据会先写入发送缓冲区
::::::等待socket可写时才实际发送
::每个客户端在nSendSleep(毫秒)时间内
::最大可写入nMsg条Login消息

::每条消息100字节（Login）
set cmd=%cmd% nMsg=1
set cmd=%cmd% nSendSleep=100

::客户端发送缓冲区大小（字节）
set cmd=%cmd% nSendBufSize=81920

::客户端接收缓冲区大小（字节）
set cmd=%cmd% nRecvBufSize=81920

::检查接收到的服务端消息ID是否连续
set cmd=%cmd% -checkMsgID

::启动程序 传入参数
F:\AA\bin\x64\Release\Client.exe %cmd%

pause