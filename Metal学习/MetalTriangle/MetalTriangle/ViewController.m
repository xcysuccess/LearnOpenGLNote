//
//  ViewController.m
//  MetalTriangle
//
//  Created by tomxiang on 2017/5/11.
//  Copyright © 2017年 tomxiang. All rights reserved.
//  http://metalbyexample.com/up-and-running-1/
//  http://www.jianshu.com/p/5ce9f4f11637
//  http://www.jianshu.com/p/65ba542b5acc
//  http://metalbyexample.com/instanced-rendering/
//  http://metalbyexample.com/up-and-running-2/

//1 创建一个MTLDevice
//2 创建一个CAMetalLayer
//3 创建一个Vertex Buffer
//4 创建一个Vertex Shader
//5 创建一个Fragment Shader
//6 创建一个Render Pipeline
//7 创建一个Command Queue
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#import "ViewController.h"

@interface ViewController ()
@property(nonatomic,strong) id <MTLDevice> device;//MTLDevice想象成连接到GPU的桥梁。创建的所有Metal对象都使用这个MTLDevice
@property(nonatomic,strong) CAMetalLayer *metalLayer;

@property(nonatomic,strong) id<MTLBuffer> vertexBuffer;
@property(nonatomic,strong) id<MTLRenderPipelineState> renderPipelineState;
@property(nonatomic,strong) id<MTLCommandQueue> commandQueue;
@property(nonatomic,strong) CADisplayLink *displayLink;

@end

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, //Left
    0.5f, -0.5f, 0.0f,  //Right
    -0.5f,  0.5f, 0.0f,   //Top
//    0.5f,  0.5f, 0.0f   //Top Right
};

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    //1.创建一个MTLDevice,connect GPUDriver and GPUhardware directly
    _device = MTLCreateSystemDefaultDevice();

    //2.创建一个CAMetalLayer
    _metalLayer = [CAMetalLayer layer];
    _metalLayer.device = _device;
    _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;//为Blue, Green, Red和Alpha提供8字节
    _metalLayer.framebufferOnly = YES; //苹果鼓励将framebufferOnly设置为true，来增强表现效率。除非你需要对从layer生成的纹理（textures）取样，或者你需要在layer绘图纹理(drawable textures)激活一些计算内核，否则你不需要设置。（大部分情况下你不用设置）
    _metalLayer.frame = self.view.layer.frame;
    [self.view.layer addSublayer:_metalLayer];
    
    //3.创建一个Vertex Buffer
    GLuint dataSize = sizeof(vertices) * sizeof(vertices[0]); //获取vertex data的字节大小
    _vertexBuffer = [_device newBufferWithBytes:&vertices length:dataSize options:MTLResourceCPUCacheModeDefaultCache];//在GPU创建一个新的buffer，从CPU里输送data
  
    //4&&5.创建一个Vertex Shader&&Fragment Shader
    id <MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
//    id <MTLFunction> vertextProgram = [defaultLibrary newFunctionWithName:@"basic_vertex"];
//    id <MTLFunction> fragmentProgram = [defaultLibrary newFunctionWithName:@"basic_fragment"];
    id <MTLFunction> vertextProgram = [defaultLibrary newFunctionWithName:@"myVertextShader"];
    id <MTLFunction> fragmentProgram = [defaultLibrary newFunctionWithName:@"myFragmentShader"];
    
    //6.创建渲染管线
    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [MTLRenderPipelineDescriptor new];
    pipelineStateDescriptor.vertexFunction = vertextProgram;
    pipelineStateDescriptor.fragmentFunction = fragmentProgram;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    
    NSError *error = nil;
    _renderPipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
    NSAssert(error == nil, @"Error must not nil");
    
    //7.GPU一次要执行的命令
    _commandQueue = [_device newCommandQueue];
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawloop)];
    [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) drawloop{
    @autoreleasepool {
        [self render];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void) render{
//    1 创建一个Render Pass Descriptor
    id<CAMetalDrawable> drawable = _metalLayer.nextDrawable;
    MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor new];
    renderPassDescriptor.colorAttachments[0].texture = drawable.texture;//nextDrawable()方法，它会返回需要绘制的纹理，使其显示在屏幕上
    renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;//loadAction设置为清除，这意味着“在绘制之前，将纹理设置为透明色”
    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(1.f, 0, 0, 1.f);
    
//    2 创建一个Command Buffer
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    
//    3 创建一个Render Command Encoder
    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    [renderEncoder setRenderPipelineState:_renderPipelineState];
    [renderEncoder setVertexBuffer:_vertexBuffer offset:0 atIndex:0];
    /**
     绘制图形
     - parameter type:          画三角形
     - parameter vertexStart:   从vertex buffer 下标为0的顶点开始
     - parameter vertexCount:   顶点数
     - parameter instanceCount: 总共有1个三角形
     */
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3 instanceCount:1];
    [renderEncoder endEncoding];
//    4 提交Command Buffer的内容
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
}

@end
