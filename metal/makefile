test: main.swift CircleAlg.swift circlealg.metal
	xcrun -sdk macosx metal circlealg.metal
	swiftc -O -framework CoreGraphics -framework Metal -framework Foundation *.swift -o circlealg
