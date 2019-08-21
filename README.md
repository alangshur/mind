# mind-engine

## Engine Role

The engine application will carry out the following tasks:  

- Register the addition of new contributions to the platform  
- Register changes to already existing contributions in the platform  
- Store the ELO ratings of all contributions to the platform in memory  
- Actively persist ELO rankings to external database storage
- Monitor distirbution of ELO scores and signal end of engine round  

## Useful Commands

Kill all mind-engine processes:

```bash
ps | grep "./mind-engine" | cut -d ' ' -f1 | xargs kill
```
