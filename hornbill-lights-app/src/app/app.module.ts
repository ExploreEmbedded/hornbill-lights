import { NgModule, ErrorHandler } from '@angular/core';
import { IonicApp, IonicModule, IonicErrorHandler } from 'ionic-angular';
import { MyApp } from './app.component';
import { HomePage } from '../pages/home/home';
import { ConnectPage } from '../pages/connect/connect';
import { HornbillBlueService } from './hornbill-blue.service';
import { ColorPalletePage } from '../pages/color-pallete/color-pallete';


@NgModule({
  declarations: [
    MyApp,
    HomePage,
    ConnectPage,
    ColorPalletePage
  ],
  imports: [
    IonicModule.forRoot(MyApp)
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyApp,
    HomePage,
    ConnectPage,
    ColorPalletePage
  ],
  providers: [{provide: ErrorHandler, useClass: IonicErrorHandler}, HornbillBlueService]
})
export class AppModule {}
