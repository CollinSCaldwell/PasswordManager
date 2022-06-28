# PasswordManager
Local password manager meant to replicate with very limited functionality those like LastPass with a master password

IF YOU FORGOT THE MASTER PASSWORD, JUST DELETE THE TEXT IN THE PASSWORDS.TXT


I was curious as to the various different cryptographic methods keeping us safe on the internet and beyond that are currently in use
I learned the basics of some like Diffie-Hellman key exchange and RSA encryption while in a discrete mathematics course and thought it would be
interesting to try programming some for myself.

I entirely understand that my algorithms are not at all secure or at all original, and under no circumstances should they be trusted to 
accurately store and keep actual passwords safe

This program uses my own version of a hash function and my own version of a symmetric encryption algorithm using block ciphers

From what I can tell the hash algorithm succeeds and is not agebraically solvable, but the symmetric algorithm might be as every 
permutation I do is linear and there are no non-linear aspects.

The biggest downside to this project is early on I started using string representations of binary numbers and while it works, is very slow.
This is especially noticable when using the multiply binary function and would be the main thing I would like to change in this program if I were to come back.

I would also like to add salt and peppering to the program as that is something I understand, but never got around to
Additionally I would like to explore having multiple users with file names as the password hashes but I would need to look into the security of that

The main vulnerability this program has is the fact that the key used for decrypthing the passwords, but not the master password itself, is kept in RAM.
My thought process behind this is simply that, if someone has access to your memory, they probably could just have a key logger.


Overall it was a very fun project and I do look forward to further exploring cryptographic algorithms and trying assymetric encryption next!
