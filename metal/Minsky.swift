import Foundation
import Metal

class Minsky {
    static let len = 1 << 19
    static let bufferlen = Minsky.len * MemoryLayout<Int32>.stride
    let device: MTLDevice
    let minskyKernelPSO: MTLComputePipelineState
    let commandQueue: MTLCommandQueue
    var x: MTLBuffer?
    var y: MTLBuffer?
    var cnt: MTLBuffer?
    var nextval: Int = 11000000 ;
    var ival = Array(repeating: 0, count: len)
    init?(_ device: MTLDevice) {
        self.device = device
        let defaultLibrary = self.device.makeDefaultLibrary()
        if (defaultLibrary == nil) {
            NSLog("Could not find library.")
            return nil
        }
        let minskyKernel = defaultLibrary!.makeFunction(name: "minsky")
        if (minskyKernel == nil) {
            NSLog("Could not find function.")
            return nil
        }
        do {
            try self.minskyKernelPSO = self.device.makeComputePipelineState(function: minskyKernel!)
        } catch {
            NSLog("Could not create pipeline")
            return nil
        }
        self.commandQueue = self.device.makeCommandQueue()!
    }
    func prepareData() {
        self.x = self.device.makeBuffer(length: Minsky.bufferlen, options: MTLResourceOptions.storageModeShared)!
        self.y = self.device.makeBuffer(length: Minsky.bufferlen, options: MTLResourceOptions.storageModeShared)!
        self.cnt = self.device.makeBuffer(length: 2*Minsky.bufferlen, options: MTLResourceOptions.storageModeShared)!
        self.generateInitialData(self.x!, self.y!, self.cnt!)
    }
    func generateInitialData(_ x: MTLBuffer, _ y: MTLBuffer, _ z: MTLBuffer) {
        let xPtr: UnsafeMutablePointer<Int32> = x.contents().assumingMemoryBound(to: Int32.self)
        let yPtr: UnsafeMutablePointer<Int32> = y.contents().assumingMemoryBound(to: Int32.self)
        let cntPtr: UnsafeMutablePointer<Int64> = z.contents().assumingMemoryBound(to: Int64.self)
        for index in 0 ..< Minsky.len {
            xPtr[index] = Int32(self.nextval)
            yPtr[index] = Int32(((self.nextval)*15)>>1)
            cntPtr[index] = 1
            self.ival[index] = self.nextval
            self.nextval += 1 ;
        }
    }
    func sendComputeCommand() {
        while (true) {
           let commandBuffer: MTLCommandBuffer = self.commandQueue.makeCommandBuffer()!
           let computeEncoder: MTLComputeCommandEncoder = commandBuffer.makeComputeCommandEncoder()!
           let t1 = CFAbsoluteTimeGetCurrent();
           self.encodeAddCommand(computeEncoder)
           computeEncoder.endEncoding()
           commandBuffer.commit()
           commandBuffer.waitUntilCompleted()
           let t2 = CFAbsoluteTimeGetCurrent() - t1;
           print("Computation completed in ", t2);
           self.scanResults()
       }
    }
    func encodeAddCommand(_ computeEncoder: MTLComputeCommandEncoder) {
        computeEncoder.setComputePipelineState(self.minskyKernelPSO)
        computeEncoder.setBuffer(self.x, offset: 0, index: 0)
        computeEncoder.setBuffer(self.y, offset: 0, index: 1)
        computeEncoder.setBuffer(self.cnt, offset: 0, index: 2)
        let gridSize: MTLSize = MTLSizeMake(Minsky.len, 1, 1)
        var threadGroupSizeInt: Int = self.minskyKernelPSO.maxTotalThreadsPerThreadgroup
        if (threadGroupSizeInt > Minsky.len) {
            threadGroupSizeInt = Minsky.len
        }
        let threadGroupSize: MTLSize = MTLSizeMake(threadGroupSizeInt, 1, 1)
        computeEncoder.dispatchThreads(gridSize, threadsPerThreadgroup: threadGroupSize)
    }
    func scanResults() {
        let x: UnsafeMutablePointer<Int32> = self.x!.contents().assumingMemoryBound(to: Int32.self)
        let y: UnsafeMutablePointer<Int32> = self.y!.contents().assumingMemoryBound(to: Int32.self)
        let cnt: UnsafeMutablePointer<Int64> = self.cnt!.contents().assumingMemoryBound(to: Int64.self)
        for index in 0 ..< Minsky.len {
            if (y[index] == 0) {
               print(self.ival[index], cnt[index], x[index])
               x[index] = Int32(self.nextval)
               y[index] = Int32((self.nextval*15)>>1)
               cnt[index] = 1
               self.ival[index] = self.nextval
               self.nextval += 1 ;
            }
        }
    }
}
