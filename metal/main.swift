import Foundation
import Metal

let device = MTLCreateSystemDefaultDevice()!
let minsky: Minsky = Minsky(device)!
minsky.prepareData()
minsky.sendComputeCommand()
