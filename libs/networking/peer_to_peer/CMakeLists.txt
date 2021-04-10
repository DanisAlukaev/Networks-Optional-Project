set(peer_to_peer_files
        peer2peer.c
        peer2peer.h)

add_library(PeerToPeer ${peer_to_peer_files})
add_subdirectory(../client build_client)
add_subdirectory(../server build_server)

target_link_libraries(PeerToPeer Client)
target_link_libraries(PeerToPeer Server)
target_link_libraries(PeerToPeer LinkedLists)