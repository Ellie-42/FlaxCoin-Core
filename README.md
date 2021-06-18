# FlaxCoin Core
 This is the core library of FlaxCoin. It should be used as a library for any external development such as GUI Wallets, mining pools, etc.

Features:
 1 minute block time
 difficulty adjustment every 5 blocks
 Quantum Secure signature scheme (SPHINCS, based on SPHINCSPLUS, modified to suit the needs of this coin)
 ASIC proof algorithm
  The rotating hash scheme ensures the hash will change with every block, and for every interal hash.
  Two hashes comprise the final "signature" hash
  Trice hashed + once hashed to create the aboce "signature" hash
  The rotating scheme also ensures anyone attepting a 51% attack would be seen quickly as the hashes will not line up properly. Thus, the community as a whole can abandoned that chain.
 The supply will be capped at 7 billion coins. Less than the US Dollar supply at the turn of the 20th century.
 Block rewards begin at250, but then decrease by half every
 
 Other built in features:
  Don't know, but the coin will be cool
