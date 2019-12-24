::关闭输出
@echo off

::服务端IP地址
set cmd="strIP=any"

::服务端端口
set cmd=%cmd% nPort=8099

::消息处理线程数量
set cmd=%cmd% nThread=1

::客户端连接上限
set cmd=%cmd% nMaxClient=10000

::客户端发送缓冲区大小（字节）
set cmd=%cmd% nSendBuffSize=81920

::客户端接收缓冲区大小（字节）
set cmd=%cmd% nRecvBuffSize=81920

::收到消息后将返回应答消息
set cmd=%cmd% -sendback

::提示发送缓冲区已写满
::当出现sendfull提示时，表示当次消息被丢弃
set cmd=%cmd% -sendfull

::检查接收到的客户端消息ID是否连续
set cmd=%cmd% -checkMsgID

::自定义标志 未使用
set cmd=%cmd% -p

F:\AA\bin\x64\Release\Server.exe %cmd%

::暂停一下
pause