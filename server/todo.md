x Convert input to use keycode
x Add collider shape to snapshot data and dynamically instantiate based on that
x instance another cube when a player joins instead of randomly assigning one. This should start in a random location and fall onto the current OR Add connected player check to simulation and ignore any players exceeding simulation count
x Add input command to reset the simulation
x Fix gravity/feel of the simulation
x Only create a snapshot when we have at least one packet sent, otherwise ignore the snapshot
x add server running message
x convert everything to use unique_ptr
x test multiple clients
x remove hard coded paths from cmakelists
x connect IDE to WSL and convert sockets to linux
x dependency injection

GSL
dependency resolver

set up github repo
Add number of chunks to message packets and only ack the message when all chunks are received

Add keep-alive packet and ack
Add client timeout – maintain time of last packet received and destroy clients with no recent acks or keep-alives (30 seconds?)

Add client-side message packet list – discard old packets to avoid object jumping. If we receive an old packet, don’t bother ack-ing it either.

Client-side prediction
Client-side entity interpolation
Client-side input buffer to smooth simulation
Client-side network simulation to simulate packet loss, jitter, and latency



/ Add cross-platform code for sockets
/ Automatically reset the simulation if no connected clients
/ Convert to windows service or set a startup task.
/ Deploy to AWS or Kirintor server