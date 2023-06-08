import socket
import textwrap


test_cases = {
    "": "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709",
    "foobar": "8843D7F92416211DE9EBB963FF4CE28125932878",
    "Hello, World": "907D14FB3AF2B0D4F18C2D46ABE8AEDCE17367BD",
    "The quick brown fox jumps over the lazy dog": "2FD4E1C67A2D28FCED849EE1BB76E7391B93EB12",
    "a " * 50000: "49A80174FA692B366216DBEFE94851ACBA042410",
}


def send_data(data: str) -> str:
    HOST, PORT = 'localhost', 8001
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        try:
            sock.connect((HOST, PORT))
            sock.sendall(bytes(data + "\n", "utf-8"))
            received = str(sock.recv(1024), "utf-8")
        except socket.error as e:
            print("Socket error: ", e)
        except Exception as e:
            print("Other exception: ", e)
        finally:
            sock.close()
    return received.strip()


def test() -> bool:
    results = []
    for data, expected in test_cases.items():
        print(f"- Test-case: '{textwrap.shorten(data, width=25)}'")
        received = send_data(data)
        print(f"  Expected: {expected}")
        print(f"  Received: {received}")
        result = received == expected
        print(f"  Result: {'[ OK ]' if result else '[FAIL]'}")
        print()
        results.append(result)
    return all(results)



if __name__ == "__main__":
    import sys

    passed = test()
    print("\n*** All tests passed! ***" if passed else "*** Some tests failed! ***")

    sys.exit(0 if passed else 1)
