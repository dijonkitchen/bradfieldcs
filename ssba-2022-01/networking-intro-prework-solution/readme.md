"simple.go" and "thorough.go" provide two different solutions to this problem, the latter parsing and verifying more fields for the sake of demonstration.

To test: `go run thorough.go && diff expected.jpeg out.jpeg` should log output and return 0.
