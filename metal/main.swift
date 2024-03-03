import Foundation
import Metal

let device = MTLCreateSystemDefaultDevice()!
let circleAlg: CircleAlg = CircleAlg(device)!
circleAlg.prepareData()
circleAlg.sendComputeCommand()
