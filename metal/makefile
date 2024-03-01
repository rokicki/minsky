test: main.swift Minsky.swift minsky.metal
	xcrun -sdk macosx metal minsky.metal
	swiftc -O -framework CoreGraphics -framework Metal -framework Foundation *.swift -o minsky
