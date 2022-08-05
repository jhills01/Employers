# NOTE: Last # of 101 == NO REROLLS!!
# NOTE: Having PC and NPC with same stats, but one allows rerolls 
#		... while the other doesn't, is a great way to see differences
#		... in reroll/no reroll probability distributions!

# type ("pc" or "npc"), fullDiceQty, fracDieNum, successNum, rerollNum
#pc,                      5,           0,         70,         90
#pc,                      5,           0,         70,         70
pc,                      5,           70,         70,         101
#pc,                      6,           70,         70,         101

# type ("pc" or "npc"), fullDiceQty, fracDieNum, successNum, rerollNum
#npc,                     5,           0,         70,         101
#npc,                     5,           0,         70,         101
npc,                     4,           0,         70,         101
#npc,                     4,           80,         70,         101



# specify the number of desired threads
# "totalDicePoolRolls" must be divisible by "threads"!!!
threads, 100