#include <iostream>
#include <cassert>
#include "websocket.h"
#include <chrono>
#include <thread>

int main(){
    std::cout << "Synchronous Websocket Test..." << std::endl;
    std::cout << "Test Complete." << std::endl;

    const Address address("ws://test.com:8080");
    assert(address.tls() == false);
    assert(address.host() == "test.com");
    assert(address.port() == 8080);
    const Address address2("wss://test.com:8080");
    assert(address2.tls() == true);
    assert(address2.host() == "test.com");
    assert(address2.port() == 8080);
    const Address address3("wss://test.com");
    assert(address3.tls() == true);
    assert(address3.host() == "test.com");
    assert(address3.port() == 443);
    const Address address4("ws://test.com");
    assert(address4.tls() == false);
    assert(address4.host() == "test.com");
    assert(address4.port() == 80);
    const Address address5("ws://test.com/foo");
    assert(address5.tls() == false);
    assert(address5.host() == "test.com");
    assert(address5.port() == 80);
    assert(address5.resource() == "/foo");


    Client c(std::string("wss://echo.websocket.org/"));

    for(size_t i = 0 ; i < 10; i++){
        c.write(std::to_string(i));
        for(const auto &m: c.read()){
            std::cout << m << std::endl;
        }
    }

    c.write(
"7c c4 2f 04 5d ea 6d 93 c6 18 96 0f 50 9c c4 4e\
7a 76 37 df 82 5d 9b fb 84 1b 2f fb 57 03 41 80\
49 26 65 c5 95 cd e8 e8 6e 03 51 2d 44 ab 64 69\
8b e3 09 b5 22 7a 22 86 e6 8f df 5b 1a ce da 30\
99 61 1a fc 73 99 a8 21 db 05 ae a6 71 33 c3 ee\
62 92 68 11 22 59 40 06 9b c6 58 06 e8 c2 26 2c\
52 7c b7 69 ea 4d bc 4d 1a 97 50 ce 23 a4 90 e2\
59 3e 6d db 5c f3 35 27 90 88 a0 dc fe 27 88 e1\
1a c0 ad 92 15 48 6d f1 f9 fc 1c 8e 8b 82 3c af\
f4 30 71 15 82 07 b2 56 56 5d dd be 5a ed f3 e8\
f8 d6 b0 28 78 ff 8c d3 4f 17 05 8e 3f 2d 89 c7\
0c a2 05 7c fa df 52 32 05 08 2d e2 3a 5b ef 9c\
17 57 ff 85 eb df 03 b7 b1 d1 99 0d 8b 7f 6b 2b\
cd f8 45 02 d0 7d 45 7c d9 39 5e 45 3e 61 da ad\
09 de bc 53 ab 53 f0 7e 56 41 3d 10 ab 57 06 9c\
df 31 45 66 26 fd a2 fe 53 c0 43 c5 97 c5 ad 9d\
5c 9d 69 e5 54 77 77 e9 07 67 b2 77 34 79 df d0\
ca 16 c0 56 d5 4b c5 c9 44 bd 02 f0 c6 b6 58 9c\
47 1a 09 5b 02 19 5b a4 f1 8a 2c 95 69 76 15 be\
ed 2a a2 57 fe 02 d3 08 a6 57 bf 7c 20 1d a9 39\
c4 49 98 3b 00 0a ea 02 09 fd 8c 5b bc 5d 0f f2\
3b 71 14 33 64 bd a1 5e 7a 18 cc 7d 94 9b 14 88\
bf db d7 43 b4 c1 c2 c0 3e 0a 98 0c cc 82 2c 9e\
16 b1 3f e0 35 0d 95 0d 3d 1c b9 60 63 8f b0 73\
0f 1f 7b 8f 52 9c 90 c1 8b 3d 14 f7 42 64 d0 0e\
d8 2d 27 3e 58 30 5d b0 dd 00 5d 59 35 a3 6b 87\
51 33 98 10 07 7e c6 f4 17 bc ff cb 47 05 40 69\
94 aa e7 cc 1c 14 ef 98 b3 c5 a5 de 6a 26 02 7d\
c7 89 27 a2 e3 45 07 18 f9 31 bf 01 c8 69 16 ae\
31 66 c5 9f 8a 39 2f c6 52 d8 99 5d 7c b6 7c 4f\
19 5b 67 44 e6 1a f7 60 30 78 cf 13 1e dc f1 85\
a1 0d d4 c6 6b c9 25 a5 a4 46 fd 5f bc a0 0f e4\
5f 3b 27 48 17 74 44 b2 c4 b6 08 2e 21 20 87 dc\
44 26 ee e9 e1 c2 2b 44 bf 46 dc 6f 03 a0 10 04\
eb 21 c3 58 8d 07 4f 3f e0 cd f9 2a b8 b9 57 74\
50 4b 3c ee f2 c2 9d 34 98 20 3f 7e 74 92 61 fb\
2e ca 38 17 9f 6f 0b 6c ea b8 c9 6e f0 61 32 cf\
96 2c 0e 2d 87 5a 9b 28 ed 9d e9 aa 0b 6f f9 30\
e5 26 df 92 4a e8 f1 7b 4e 4a 54 ef cf 92 ca 4b\
1d 7b e8 02 e7 fb 4d 9d ce 19 27 63 6c 8c 60 b8\
6e ff 6e fb 50 3a cd 18 10 11 76 b1 52 4d fd 06\
64 2a 48 97 20 ef ba 7c 59 81 50 34 06 cd be a3\
ef 93 98 fb 52 b2 8a e6 a7 ad 1b ae d2 be 0b af\
db b8 58 0d f1 08 53 ba f3 1d c9 67 3f 32 64 c8\
52 73 aa fe f5 ba bb 8f 77 4f 4b 2c 98 6b 5e 00\
f9 e8 10 4c c5 84 01 33 c5 20 ae 8d e6 13 eb 9e\
78 c2 ab 46 3f 51 8d 48 c9 6f 00 f6 21 c7 cd ac\
91 43 8a 8d 12 24 1b 35 4d b5 e3 02 ec 1e de a6\
71 7d b5 08 81 36 e9 39 b0 b0 46 bd 8b 71 65 48\
24 a0 aa f0 cc 93 a1 64 9a 15 a5 12 3e 79 cc 81\
5d 92 c8 74 c7 3a dd a8 e9 96 a9 df 9f 3b 92 9b\
39 9c cf 81 58 d8 de 3f b3 37 e2 09 d5 02 67 04\
19 27 a5 32 d8 0b e6 82 51 fd 37 55 58 7e 9e 3f\
e1 12 a7 26 5b b6 f0 24 9f 73 bb be c6 e2 64 4c\
ca 4e ea 9e 83 44 1c c1 f3 e5 c4 2e 5c 88 03 f2\
ae ea 7f 46 52 f0 1d 63 2a de 92 a3 8b ef 5e a8\
34 d0 b6 7b 14 76 ab c7 a8 5b d7 f5 cb 3c 32 c8\
03 c7 2f ba c1 dc 19 4e 21 a8 30 37 d5 ef 6a 75\
74 25 6f 60 7a 40 68 bd 6d 7d 39 17 f5 e3 54 c2\
6c 47 88 94 54 69 93 c3 8a b2 9c 80 3c 3c 68 57\
9e 1e d1 98 46 96 6b 33 aa b5 57 2b 5f c6 85 80\
f9 14 b6 6f 47 5f ae 24 b2 74 b3 20 06 2a 4e a1\
48 a2 ba c3 42 9a cd 11 6f 2b 7b 59 ff df 7e 31\
99 28 d7 98 d0 fc 1f f5 39 bb 01 10 c5 2d 8d 03\
02 6c 96 27 6c 32 b5 9d 9e b3 8d 1b a3 e1 17 e1\
ea 94 28 e6 93 1f 01 b7 7c 27 1f 58 35 39 40 28\
24 31 63 73 33 71 c1 89 1c af f2 a4 d1 d4 7a f2\
5d ee f4 0c 39 98 62 a6 6b e5 30 91 f3 c4 98 0c\
a1 1a 49 0a 91 f8 9b d7 73 a4 85 2e a8 b0 63 dd\
b6 68 66 b6 54 c1 8c 0b 63 e7 7a 8d 21 c0 46 e9\
54 17 1a b0 43 6a b3 15 fe 03 dd 2f 01 3f 17 3f\
0e 5b ba ea 5d 1b ab f7 e3 1e 1e b1 f5 60 10 46\
73 fe 29 ea 6a 72 a2 f5 29 8e 85 5c 98 22 61 8e\
c7 81 9a 64 8a d0 30 a1 51 d4 6e cd de 30 7a c9\
39 25 06 23 10 f6 77 ed a4 60 cf 3e 67 c5 e9 0b\
6f dd 11 8d 9b ef bc d9 2a d7 f8 cc b5 a6 17 52\
b0 f6 54 a4 81 98 d8 f9 0f b1 96 4d c1 87 f4 95\
c2 22 ed 62 12 a6 79 8d 15 c3 94 a9 5d 24 60 7f\
23 38 51 99 46 2c 87 51 50 50 f3 64 f1 bf d8 d4\
c2 41 65 45 e0 49 00 1f 38 15 1b a3 0b f3 4d 91\
c0 3b fd 4b 0a 8d 6d 68 1b 7b 07 49 69 50 36 52\
59 c5 d4 6e a9 5c e3 9c 17 b5 75 dd d1 16 5c 53\
e9 d8 f0 2f 8d e0 ac 34 32 ea 9c 90 c7 71 fe ed\
08 bb 3a 16 1b e9 ad 91 73 e8 67 f6 25 f4 83 63\
ea a2 07 82 ec 86 da a2 dd c2 a5 2a 08 c9 5e 0e\
0b 6a 1c a8 cc 59 ac 1a 9f 46 81 53 09 a4 67 18\
d1 dd 08 d8 a5 50 31 cc cf 0f a0 82 24 2e 35 11\
56 02 14 8f 41 6f 63 84 7f 14 d3 30 de 03 3e 20\
84 a3 ca 36 72 c3 45 2e d9 0c 30 19 72 83 51 13\
24 c1 89 d4 83 e8 0a 45 da 57 e4 2c b8 c4 e6 42\
2d e1 84 97 1b 2f 06 a5 65 9e 05 6a 58 ad 53 a6\
96 b3 36 e0 27 5a 16 48 bf 6a a2 c7 d6 7c 71 aa\
2d b6 16 54 32 d1 1b 00 92 c9 cc ca e3 2a 7f ef\
e7 5b 5f bf d6 ca 4b 1b 86 c8 9a b1 7f cd 24 b1\
f0 7e 11 44 73 0f 58 99 68 fd 98 b6 a8 97 b3 ee\
c6 df 1e 2d c0 2d de 23 3d df 74 a8 30 36 7d 42\
b7 72 21 82 5c a1 5d b9 f8 5e c7 60 32 a5 e3 f0\
57 eb ba 16 fa d9 48 67 3e 89 a6 77 0e 72 38 0e\
47 0a a3 1a a6 7c d7 e7 82 a2 09 fa fa 6c fd 51\
6d 9b 74 28 c5 f1 95 9d 03 dd be b5 22 b4 e4 61\
a9 c9 a3 dd 51 c2 56 ad 41 27 81 fa eb 0f 8a aa\
3f 3f c2 34 4e 06 3c 9b 9d f0 55 b0 26 af 18 30\
ac a1 6e 84 96 2e b2 c7 0c 31 84 ca 9b f3 7f fe\
dc f4 d3 7d db 75 6e 9b 62 8f d9 2e cd 6a 75 d3\
fa 8b 05 61 3d a3 21 b3 7d 08 16 86 ed ed c2 34\
3d c0 29 da 1e 57 2e dc e7 63 79 94 21 b6 c4 77\
90 fc 88 c1 5d b6 64 4b 97 8f 00 3f 5d c2 e2 79\
f5 82 c7 7a 58 fb 5f 2a ed 9f 71 a0 1c 4f ca 96\
c8 8e 1d e7 4a a8 6b 94 49 52 6e f4 70 6e 21 61\
a9 1d 73 5f a1 36 af 26 14 0f e0 27 d7 66 4d 0b\
f0 3f 22 68 e3 fd da 25 f5 ec 0d 07 b0 2f 3e ad\
a3 4d 3c f1 ed 45 6a ff a3 3f e4 17 8a 42 37 45\
56 bd 37 c1 5c eb e3 a7 7f 68 c2 47 9d a1 ac 73\
60 21 8c b7 5b 1a 59 3d e6 e5 c1 ed 4e 59 8c 97\
79 d7 3e c7 37 87 3a 77 07 4a d5 45 12 8d 0c c7\
4d 89 1f dc 00 e1 d1 ec bc 78 c5 96 ed 4c 6f 51\
b6 c8 3b 94 f0 33 34 2b bd 8b 18 90 38 04 9e 03\
47 9a 29 c5 62 f1 75 c9 17 dd 8f 2b bd 4f 6c a4\
f7 73 c3 1c 81 79 fb 5b 65 78 38 8f 4b 44 71 48\
75 c8 37 e4 0e 28 a1 5b 4d 83 f7 41 23 3d 23 56\
a0 60 3d 90 69 20 f2 23 c6 00 d2 19 d1 af d8 97\
3e 46 95 74 33 43 21 8e 9f fe 10 5b ee e7 99 fa\
14 bd 33 f8 86 a9 d1 4e ad ad 0a 05 59 f4 ae f2\
28 d3 37 ea 85 9a ae 5b dc ab fb ae 8a a3 b5 78\
76 68 b8 a4 62 09 ed 5f 14 a5 a7 5b a1 9c 3b cb\
a4 06 e2 95 a5 22 74 d9 78 47 09 41 72 33 ca c3\
9d fa a4 ad e0 78 73 38 b5 53 25 e2 87 a2 98 c0\
67 11 8d 8c 73 de c8 fe 50 a9 4f c1 4a fe dd 37\
6f 27 6c f3 52 1f c3 25 15 d9 33 95 fb 53 fd ea\
7e 00 61 d4 d9 19 e2 59 fc 7c fa d4 94 28 64 52\
94 d8 29 44 a8 ad f5 78 03 84 9a af e1 03 4d 92\
51 9c 73 6b 13 e6 96 a4 e7 11 59 d8 81 3d a0 7e\
db 30 88 ec ed 9c cc 8e 13 d3 1c 18 e6 93 a9 90\
07 1f 44 39 39 cd 5e b7 54 77 21 ae fd d4 f6 19\
e9 8c d3 8d 37 12 b6 b4 57 96 af 2b 8b dc d8 d6\
6a c9 e4 f7 d8 61 32 44 c2 8e eb ab b0 71 83 4f\
a2 59 82 76 41 c1 cb ef c7 5e 84 e5 40 c0 15 81\
bb 73 3b 9f 9b 59 11 13 e3 66 36 64 e7 1d f4 1e\
b3 4f e3 61 f6 b2 79 c7 20 e6 80 fe 4c ef a1 1a\
aa 5c 84 3f 4b fc a2 30 1e 52 2b 6b 3e 5c 90 b3\
a9 80 10 5a ef e4 bd 85 db c0 ff ae 55 b4 d6 63\
8d 0e 18 22 66 04 c1 29 31 49 59 7a a1 dc 0e 95\
dc 71 45 84 b4 2a 2e b3 28 d0 50 aa 1b 96 34 b9\
ec a2 2a 6f 59 4c 74 e5 62 85 df e8 92 f8 52 70\
70 48 83 81 d1 08 c3 58 bc fa e5 e9 6b c4 98 86\
34 bc c6 7d b7 c7 ad 80 e7 b7 57 6f 0d 78 f0 1c\
31 ed 60 d9 bb f2 84 41 70 b6 bf 6f 68 80 2c 93\
a6 ba 52 28 2f 45 1a f8 8a d8 8d d9 87 8b e7 04\
c4 37 5e 25 65 f5 f4 03 54 f9 3e c4 94 6a 17 c4\
ee ae ba 6d 62 0c 80 45 e0 33 3f ed 3e 84 e8 56\
63 52 12 e3 6d 2a 82 22 4f 84 7e 47 1d cf 36 e1\
0b 0e 67 d3 c2 9e 6b 04 b5 9d 77 42 02 24 a9 81\
a9 d9 ff 6e cc 1d b4 64 1d 70 b2 0b 72 d7 e5 72\
8c d2 d5 b8 33 ad 7e 69 6f 01 6c c0 d4 8b 20 f8\
f2 b4 cf 26 45 a1 fa ab ab cd b6 3e 44 08 b2 d7\
c7 31 50 8d 30 cd bc 1e fc 8f 4f be aa d2 df 30\
7c 3a 76 7d 40 50 ea 70 30 de 11 d7 0b 1f 7a 4c\
24 45 6e 20 8c 1b 52 52 c2 22 d5 dd 31 16 28 43\
02 76 85 2b e9 87 3c 7e 9a ba 2a a5 d7 8e 87 b9\
c5 72 d4 e5 32 54 d4 9f ec d6 08 44 0f 99 70 fb\
f1 ce c5 44 ab 71 48 59 60 08 39 4a 13 b7 12 ca\
31 43 b1 d2 53 76 9a 3c 4e 9b a9 2c 3d a4 76 c6\
89 82 1f c4 e9 bb 0f d2 38 a2 db 77 0d fe 68 4e\
d8 2f ab 8e 9a ba cd 31 d1 6a f1 d8 24 e1 75 0d\
5d 3f 77 e6 f4 6f a5 0d 62 52 c4 f2 6e ce 47 3a\
9e 41 ee d7 1d 4a 6d 1a 55 73 40 34 ab 9b ed f2\
41 99 ef 8b a9 28 7a 1c f8 bf 35 ec de 50 5d 86\
d0 54 a4 8c cf f5 96 87 bb 09 6b 80 2d 06 01 fc\
94 f8 63 83 3d f5 26 90 55 ad f4 74 cf 49 90 d7\
7c ee cf db bc 5d 25 33 54 ed 08 e1 d1 d7 12 1c\
58 09 ef 6b 46 29 83 73 6a 95 9c c6 32 ca 17 ef\
60 38 63 3f ce 6b 63 83 82 88 11 92 13 42 a6 39\
65 25 ee 3c e2 0b 7e ad 09 2d b9 94 0d b5 12 75\
12 d7 6d ba 2b cd b0 a8 10 d6 56 92 01 0a 0d 40\
0a 1c e1 fa 77 0c 4f 65 a4 39 f7 d7 eb 90 b4 94\
dd 71 0d a2 16 21 32 94 67 53 a1 46 d2 49 fe 8e\
e1 46 dd 62 47 91 07 bc 15 59 7b e9 63 be fe b1\
8e 9d 24 49 4e 08 6f d9 61 ef 23 18 fd 2d 19 17\
50 83 86 50 e0 79 f5 e7 c5 7b 7d c7 57 08 16 04\
e1 9e 33 80 ba 2a 28 ba 60 7d 47 dc f6 e1 59 cf\
73 17 4f a4 74 77 8c ef 9a e5 68 55 0a d1 a4 10\
75 1c 3d e1 bb 2d 0e 2b f2 63 64 af b2 a0 4c 6a\
90 0b be 05 0d b0 9b 0b 10 f0 37 f0 52 65 7e 87\
80 87 1e 65 41 dd 6b 14 d8 04 0e 7a 56 80 91 5d\
91 bb a3 e0 bc 66 bb 4e 08 a4 53 12 55 65 f3 56\
b4 7e e6 b3 a6 18 71 47 a6 39 d4 ea a7 43 c3 4b\
7d 2a 18 07 3e c0 02 f1 73 67 e3 83 a9 4f be b0\
57 4d 10 bd 92 e6 0f c5 91 5e 8e 2d 22 6d 72 e6\
8c 33 38 82 e9 b3 0f f1 9f 11 5f 64 f4 2f 21 60\
56 c6 02 ea 39 c1 e5 01 50 d5 1f cb 2d 1b 52 9c\
16 4e 87 6a bb e1 20 66 2e 36 7f 2a 88 f8 4f c2\
7c e4 3e 20 f8 34 eb 17 39 3c 84 f8 66 4b 96 f5\
54 9c d6 ac 05 32 b8 76 27 43 1a 97 a1 f8 e4 f9\
2d 69 fb 77 30 30 bf ac 69 d0 eb 0d 4c 53 ff 37\
5e e3 c5 d2 53 03 98 7a 22 ab c6 e9 45 eb 42 5b\
63 93 1d ee cf 2f 66 8b 27 69 5e 41 65 bf 39 99\
76 a6 da 5d 52 65 58 74 f0 03 dc 8b a1 d4 ee 9e\
1d 09 44 b4 9b 00 7f 62 66 99 12 45 da 83 11 88\
f9 3f 4d 3c 53 2a 3e 52 eb 2b 0e 3d 9c a6 ff 1f\
3c ad 63 43 dc c8 1c e5 8b 3a 5f 38 0b a6 7c d6\
57 7b 65 a0 bc 96 6a ee 73 db b0 4c e3 d9 2f 12\
63 60 80 6e 10 a9 9b f8 21 67 4c f1 52 76 d0 66\
c1 bc aa e8 bf 36 00 1c 2a 50 78 d5 64 d9 7f 25\
ec 68 b1 ff 70 75 6e c1 68 53 8b 1c 45 06 2c 28\
4c 96 90 d5 db ad 55 2c c0 b1 29 4a e6 5f fd 75\
6e e2 e4 b4 81 fa 43 2f b2 fa b8 ea 9c b8 5f 12\
3d d7 f3 e4 05 63 ed b7 d7 0e 24 89 f0 35 b0 1c\
d1 9c 2e 18 c4 b9 29 35 22 d6 42 7f 36 cc fb c5\
37 09 a7 16 44 08 87 3b 2c 3d e7 fe 4b 7d 00 5e\
2c a5 a9 ee 46 5e f3 1a 9f d5 2a 0e b1 1b e0 0e\
61 f2 c0 ac 48 e5 b5 44 9e 1e 9b 63 4f 01 f0 48\
ee ee 27 b5 58 fc 51 1e ab e3 1f e5 ab db 87 cb\
ff f1 f8 e5 5a c6 c5 23 99 aa 6a 6b 3e 8a 8d c7\
5d e9 41 53 a2 9a dd 94 9f 81 33 12 19 e4 f3 68\
4f d4 34 f2 41 cf aa 28 7d 1d 73 be 72 ea e1 3c\
fe 5c 4a e1 cc 53 94 ef 9a 00 17 47 a3 f1 b0 92\
68 18 5b 23 c4 a9 84 52 86 96 06 4b bb d3 64 33\
6b ae 78 c9 77 02 71 c8 2c 53 86 45 22 43 45 d6\
8f 4e 42 15 cd 00 36 bd 5e 45 0d 11 47 c1 72 98\
fc b2 3b a3 b0 c8 4d 79 61 bf d4 39 02 58 dc a6\
f7 02 f5 b3 56 e1 21 25 54 c5 45 4b 52 d6 09 00\
94 0d 55 9f 98 56 4c db 49 1b ea f4 b7 8e 1d 39\
2d 66 62 5f 54 5c 63 0b bf e1 1b 15 e3 2b 42 2a\
fc 44 ea fb cf cd 67 7b 60 87 d3 18 5d ba 5a b6\
d9 46 04 bd 87 90 b9 ab ec 91 6b 4e ff 9f 2e 4b\
be fe 89 53 bb 3b 03 e7 c7 85 ba cd 39 88 f8 6e\
59 8a f8 69 ee 52 cd bd bb f7 94 57 47 b5 1d ec\
a9 f7 16 23 09 6c 8c 64 59 02 26 87 10 85 10 c1\
df 14 53 ce 41 29 22 7e 42 be c5 40 e4 d0 3b 9b\
a6 cc b9 6a 1e f3 fa d7 ad 56 4a cb 67 1d 32 fb\
e4 6e 1f 83 55 5c 96 bf fe 8c aa f0 77 c2 60 5c\
1f c9 77 83 c9 18 6b c6 fe 87 7b 75 4e 02 08 30\
12 95 97 4a 15 82 1c c7 ea 6f 6a 72 66 b6 6e e6\
6d cd 90 c5 c0 5f 17 e2 d9 ce a9 f9 1b 31 f5 16\
60 a8 74 56 b7 6c c2 dd a8 f5 ee 00 ef 38 41 dd\
d9 8d 35 ea 54 ad a4 48 3d 12 43 ba 5f e7 42 87\
19 c1 5b b0 c8 79 6b 24 4c d7 98 d3 0f 80 20 3e\
1e 68 70 3c ef 22 42 cb 89 b8 46 4d a4 32 67 cb\
80 88 d5 78 e1 e6 3b 1e f4 9d f4 a0 b9 18 89 12\
b1 61 5c dc 75 eb ce 48 95 5b c7 11 db 86 c2 26\
a3 6e 67 3d 35 20 46 09 22 56 cb c2 b4 4e 44 c6\
09 32 aa d5 35 be 27 1f 41 9b 79 b4 1f 0a 47 66\
44 78 4f 8b 86 b7 fd b2 12 e6 e9 62 f9 c0 ef dc\
6c 72 72 d7 0f 97 8e 77 93 bb 1e 7e 93 49 06 1b\
ca 99 75 27 f2 01 36 23 92 f0 b8 16 4b c1 51 e5\
2f bc a7 d3 b0 19 29 26 ca 2a ca c6 0d a0 e9 b6\
20 84 eb 02 af b0 8e bd 8c cd 04 1c f5 60 12 c0\
a4 39 5a 8d 7c 3e 62 79 ed ec eb 24 14 7a fe cb\
f5 a6 cc 68 f6 1b b4 9d ac 41 28 70 b3 1d 83 9b\
c1 27 14 35 c6 a1 33 28 c6 5e 2d 45 74 b5 79 16\
02 75 e5 99 05 4c 8c 6d c6 ea 05 05 29 df ad 49\
81 14 3e 3e cd f0 a4 9a 75 ba b5 a8 e9 e0 58 2b\
d7 d2 f4 06 25 8a 63 1d 0b a1 36 47 7b 68 60 7e\
5b da 21 9b a6 e4 fc b3 6f 36 8a c7 02 65 4a cf\
03 a5 48 a8 07 7b 85 9e c0 3d 1d 83 5b 34 33 2c\
9f e8 61 96 51 f2 8e b1 ad e4 e3 e9 e5 c9 34 79");

	for(const auto &m: c.read()){
		std::cout << m << std::endl;
	}

    return 0;
}
