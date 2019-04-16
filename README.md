# Producer-Consumer (Bounded-Buffer) Problem
Developed a multithreaded program in C using POSIX Threads and semaphores to solve the producer-consumer problem. Implemented and tested various mechanisms in producer and consumer thread functions to avoid deadlock.

Press 'p' to pause, and 'r' to resume execution of the program.

## The problem statement is as follows:
- There are 3 generators, each producing a unique material
- Materials are stored in an input buffer with size 10 before forwarded to operators
- There are 3 operators with same priority and are responsible for producing products based on materials
- Each product needs 2 different materials, an operator needs 2 tools for this
- There are 3 tools provided for operators
- An operators can only process 1 product at a time
- When an operator gets both materials and tools, can produce a product within time between 0.01 to 1 second. Otherwise, must wait untila ll necessities met
- Operator can grab materials or tools first, but can only get 1 thing at a time
- If operator decides to make another product before current product, can put materials and tools back, and get new materials and tools
- All products are put into output queue of unlimited size
- An operator cannot start a new product before it puts product in output queue
- Restrictions to products:
  1) No same products can be next to each other in queue. 2 products are the same if they are made from same materials
  2) The difference of the number of any 2 kinds of products produced < 10
