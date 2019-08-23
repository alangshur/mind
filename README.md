# mind-engine

## Dependencies

The following dependencies are required for compiling and developing
this project:

- GNU g++ compiler: `$ brew install gcc`
- GNU make: `$ brew install make`
- Boost libraries: `$ brew install boost`  

## Building

To compile the project (within project directory): `$ make`  

To run and compile the project: `$ make run`

## Pinned Commands

Kill all mind-engine processes:

```bash
ps | grep "./mind-engine" | cut -d ' ' -f1 | xargs kill
```
