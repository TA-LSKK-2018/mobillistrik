import {Component, OnInit} from '@angular/core';
import {AuthService} from '../../services/auth.service';
import {Router} from '@angular/router';

@Component({
  selector: 'app-sensor',
  templateUrl: './sensor.component.html',
  styleUrls: ['./sensor.component.css']
})
export class SensorComponent implements OnInit {

  user: Object;

  constructor(private authService:AuthService, private router:Router) { }
  
  ngOnInit() {
      this.authService.getSensor().subscribe(sensor =>{
      this.user = sensor;
    }, err => {
      console.log(err);
      return false;
    });
  }

}
