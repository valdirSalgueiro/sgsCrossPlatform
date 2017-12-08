#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#import "../../OpenGLESApp1.Shared/game/Engine.h"

#import <memory>

@interface GameViewController () {}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation GameViewController
{
    std::unique_ptr<Engine> engine;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }

    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [self setupGL];
}

- (void)dealloc
{
    [self tearDownGL];

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;

        [self tearDownGL];

        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];

	if (engine == nullptr)
	{
		engine.reset(new Engine());
		engine->init(self.view.bounds.size.width, self.view.bounds.size.height);
	}
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];

	engine.reset();
}

#pragma mark - GLKView and GLKViewController delegate methods

extern float _rotation;

- (void)update
{
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	engine->update(1/60);	
	engine->render(1/60);
}

@end
